#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event.h>
#include <mysql_db.h>

bool testInsert(BtchDB* db){
    if(!db->AddAddress(4, "217.19.31.69", 8333)){
        return false;
    }

    return db->AddAddress(4, "217.19.31.62", 8333);
}

bool testEnable(BtchDB* db){
    return db->EnableAddress(4, "217.19.31.69", 8333);
}


bool testDisable(BtchDB* db){
    return db->AddAddress(4, "217.19.31.62", 8333);
}

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
    BtchDB* db = BtchDB::GetInstance();
    db->Init("bitcoin", "test");

    if(!testInsert(db)){
        printf("insert failed\n");
    }

    if(!testEnable(db)){
        printf("enable failed\n");
    }

    if(!testDisable(db)){
        printf("disable failed\n");
    }

    return 0;
}