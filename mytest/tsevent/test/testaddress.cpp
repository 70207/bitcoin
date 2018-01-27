#include <unistd.h>
#include <address_db.h>

bool testInsert(BtchAddrDB* db){
    if(!db->AddAddress(4, "217.19.31.69", 8333)){
        return false;
    }

    return db->AddAddress(4, "217.19.31.62", 8333);
}

bool testEnable(BtchAddrDB* db){
    return db->EnableAddress(4, "217.19.31.69", 8333);
}


bool testDisable(BtchAddrDB* db){
    return db->AddAddress(4, "217.19.31.62", 8333);
}


int main(int argc, char** argv)
{
    BtchAddrDB* db = BtchAddrDB::GetInstance();
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