
#include <stdio.h>

#include <file_db.h>


static BtchFileDB* g_btch_file_db = 0;

BtchFileDB* BtchFileDB::GetInstance()
{
    if(!g_btch_file_db){
        g_btch_file_db = new BtchFileDB();
    }

    return g_btch_file_db;
}

BtchFileDB::BtchLevelDB():
_db(0),
_ucDb(0)
{

	
}

bool BtchFileDB::Init(const char* path, const char* ucPath)
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





bool BtchFileDB::AddAddress(int type, const char *ip, int port)
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

bool BtchFileDB::EnableAddress(int type, const char *ip, int port)
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

bool BtchFileDB::DisableAddress(int type, const char *ip, int port)
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

bool BtchFileDB::AddUnCheckedAddress(int type, const char *ip, int port)
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