// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __BTCH_ADDRESS_DB_H__
#define __BTCH_ADDRESS_DB_H__


#include <string>


class BtchAddrDB 
{
    public:
        static BtchAddrDB* GetInstance();

        BtchAddrDB(const BtchAddrDB &) = delete;
        BtchAddrDB &operator=(const BtchAddrDB &) = delete;



        bool Init(const char* db = "bitcoin", const char* table = "address", const char* ucTable = "ucAddress");

        bool AddAddress(int type, const char *ip, int port);
        bool EnableAddress(int type, const char *ip, int port);
        bool DisableAddress(int type, const char *ip, int port);

        bool AddUnCheckedAddress(int type, const char *ip, int port);



    private:
        BtchAddrDB();


        bool ReConnect(void** conn);

        void* Connect();

        bool Execute(void** conn, const char* sql);

        long Insert(void** conn, const char* sql);

    private:
        void*      _addConn;
        void*      _enableConn;
        void*      _disableConn;

        void*      _ucAddConn;

        const char* _db;
        const char* _table;
        const char* _ucTable;


};


#endif // __BTCH_DB_H__
