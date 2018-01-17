#ifndef __BTCH_EVENT_H__
#define __BTCH_EVENT_H__

#include <unistd.h>
#include <unordered_map>
#include <functional>
#include <memory>

struct BtchEvent{
    public:
    int                                         fd;
    void*                                       data;
    std::function<void(int fd, void* data)>     read;
    std::function<void(int fd, void* data)>     write;
    std::function<void(int fd, void* data)>     error;
};

class BtchEventLoop{
    public:
        BtchEventLoop();
         ~BtchEventLoop();

    public:
        void Init();
        void AddEvent(int fd, 
            void*  data,
            std::function<void(int fd, void* data)> readFunc, 
            std::function<void(int fd, void* data)> writeFunc,
            std::function<void(int fd, void* data)> errorFunc
            );
        void DelEvent(int fd);
        void Process();

        std::unordered_map<int,  std::shared_ptr<BtchEvent>>   events;
        
        
        void* loopData;
        int  loopFd;

    private:
        void AddEventInternal(int fd);
        void DelEventInternal(int fd);

};


#endif