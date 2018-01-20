#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>

#include <sys/epoll.h>

#include <event.h>


#define BTCH_EVENTS_NUM_MAX                 1000000

BtchEventLoop::BtchEventLoop():
loopFd(0),
loopData(0)
{

}

BtchEventLoop::~BtchEventLoop()
{

}


void BtchEventLoop::Init()
{
    int epoll_fd = epoll_create( BTCH_EVENTS_NUM_MAX );
    struct epoll_event* events;

    int size = BTCH_EVENTS_NUM_MAX * sizeof( struct epoll_event );
	events = ( struct epoll_event* ) malloc( size );
	memset( events, 0, size );

    loopData = events;
    loopFd = epoll_fd;


}

void BtchEventLoop::AddEvent(int fd,  
            void* data,
            std::function<void(int fd, void* data)> readFunc, 
            std::function<void(int fd, void* data)> writeFunc,
            std::function<void(int fd, void* data)> errorFunc
            )
{

    auto it = events.find(fd);
    std::shared_ptr<BtchEvent> event = 0;
    if (it == events.end())
    {
        std::shared_ptr<BtchEvent> ev(new BtchEvent());
        event = ev;
        event->fd = fd;
        event->data = data;
        events.emplace(fd, event);
        AddEventInternal(fd);
    }
    else
    {
        event = it->second;
    }

    event->read = std::move(readFunc);
    event->write = std::move(writeFunc);
    event->error = std::move(errorFunc);
    event->data = data;
    
}


void BtchEventLoop::AddEventInternal(int fd)
{
    struct epoll_event ee;

    int type = EPOLLIN | EPOLLOUT;


    ee.data.u64 = fd;
	ee.events = type;

    epoll_ctl( loopFd, EPOLL_CTL_ADD, fd, &ee );

}

void BtchEventLoop::DelEvent(int fd)
{
    auto it = events.find(fd);
    if (it == events.end())
    {
        return;
    }

    auto jt = it->second;
    events.erase(it);

    DelEventInternal(fd);

}

void BtchEventLoop::DelEventInternal(int fd)
{   
    struct epoll_event ee;

    int type = EPOLLIN | EPOLLOUT;

    ee.data.u64 = fd;
	ee.events = type;

    epoll_ctl( loopFd, EPOLL_CTL_DEL, fd, &ee );

}

void BtchEventLoop::Process()
{
    struct epoll_event* events = (struct epoll_event*) loopData;

	int e_num = epoll_wait( loopFd, events, BTCH_EVENTS_NUM_MAX, 500);

    for(int i = 0; i < e_num; ++i){
        struct epoll_event& ev = events[i];
        int fd = ev.data.u64;
        int type = ev.events;

        auto it = this->events.find(fd);
        if(it == this->events.end()){
            DelEventInternal(fd);
            continue;
        }

        auto bev = it->second;
        

        if(type & ( EPOLLHUP | EPOLLERR )){
            bev->error(fd, bev->data);
        }
        else{
            if (type & EPOLLOUT)
            {
                bev->write(fd, bev->data);
            }

            if (type & EPOLLIN)
            {
                bev->read(fd, bev->data);
            }
        }

    }

}