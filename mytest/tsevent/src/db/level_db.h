// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __BTCH_LEVEL_DB_H__
#define __BTCH_LEVEL_DB_H__

#include <leveldb/db.h>



class BtchLevelDB 
{
    public:
        static BtchLevelDB* GetInstance();

        BtchLevelDB(const BtchLevelDB &) = delete;
        BtchLevelDB &operator=(const BtchLevelDB &) = delete;



        bool Init(const char* path, const char* ucPath);

        bool AddAddress(int type, const char *ip, int port);
        bool EnableAddress(int type, const char *ip, int port);
        bool DisableAddress(int type, const char *ip, int port);

        bool AddUnCheckedAddress(int type, const char *ip, int port);

        


    private:
        BtchLevelDB();


        bool ReConnect(void** conn);

        void* Connect();

        bool Execute(void** conn, const char* sql);

    private:

        leveldb::DB      *_db;
        leveldb::DB      *_ucDb;
   
};


#endif // __BTCH_DB_H__
