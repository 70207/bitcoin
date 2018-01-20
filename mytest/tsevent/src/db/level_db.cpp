
#include <stdio.h>

#include <level_db.h>


static BtchLevelDB* g_btch_level_db = 0;

BtchLevelDB* BtchLevelDB::GetInstance()
{
    if(!g_btch_level_db){
        g_btch_level_db = new BtchLevelDB();
    }

    return g_btch_level_db;
}

BtchLevelDB::BtchLevelDB():
_db(0),
_ucDb(0)
{

	
}

bool BtchLevelDB::Init(const char* path, const char* ucPath)
{
   if(!path || !path[0]){
       return false;
   }

   leveldb::DB      *db;
   leveldb::DB      *ucDb;

   leveldb::Status status = leveldb::DB::Open(leveldb::Options(), path,  &db); 

   if(!status.ok()){
       return false;
   }

   status = leveldb::DB::Open(leveldb::Options(), ucPath,  &ucDb); 

   if(!status.ok()){
       delete db;
       return false;
   }


    _db = db;
    _ucDb = ucDb;
   return true;
}





bool BtchLevelDB::AddAddress(int type, const char *ip, int port)
{
    char key[512];
    char value[512];

    sprintf(key, "type:%d:ip:%s:port:%d", type, ip, port);

    sprintf(value, "status:%d", 0);

    leveldb::Status status = _db->Put(leveldb::WriteOptions(), key, value);
    if(!status.ok()){
        return false;
    }


    return true;


}

bool BtchLevelDB::EnableAddress(int type, const char *ip, int port)
{
    char key[512];
    char value[512];

    sprintf(key, "type:%d:ip:%s:port:%d", type, ip, port);

    sprintf(value, "status:%d", 0);

    leveldb::Status status = _db->Put(leveldb::WriteOptions(), key, value);
    if(!status.ok()){
        return false;
    }


    return true;
}

bool BtchLevelDB::DisableAddress(int type, const char *ip, int port)
{
    char key[512];
    char value[512];

    sprintf(key, "type:%d:ip:%s:port:%d", type, ip, port);

    sprintf(value, "status:%d", 0);

    leveldb::Status status = _db->Put(leveldb::WriteOptions(), key, value);
    if(!status.ok()){
        return false;
    }


    return true;
}

bool BtchLevelDB::AddUnCheckedAddress(int type, const char *ip, int port)
{
    char key[512];
    char value[512];

    sprintf(key, "type:%d:ip:%s:port:%d", type, ip, port);

    sprintf(value, "status:%d", 0);

    leveldb::Status status = _ucDb->Put(leveldb::WriteOptions(), key, value);
    if(!status.ok()){
        return false;
    }


    return true;

}