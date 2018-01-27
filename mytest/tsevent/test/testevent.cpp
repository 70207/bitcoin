#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event.h>


void testEvent(){
    BtchEventLoop* loop = new BtchEventLoop();

    loop->Init();

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd <= 0){
        printf("open fd failed\n");
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(10002);

    socklen_t  len = sizeof(struct sockaddr);

    int flag = connect(fd, (struct sockaddr*)&addr, len);
    if(flag < 0 ){
        printf("connect fd failed\n");
        return;
    }

    loop->AddEvent(fd, 0, [](int fd, void* data){
        printf("read event");
    }, [](int fd, void* data){
        printf("write event");
        write(fd, "hello world", sizeof("hello world"));
    },
    [](int fd, void* data){
        printf("error event");
    });

    while(1){
        loop->Process();
    }
}


int main(int argc, char** argv)
{
    testEvent();
    return 0;
}