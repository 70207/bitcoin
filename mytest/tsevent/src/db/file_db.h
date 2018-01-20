// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __BTCH_FILE_DB_H__
#define __BTCH_FILE_DB_H__




class BtchFileDB
{
    public:
        static BtchFileDB* GetInstance();

        BtchFileDB(const BtchFileDB &) = delete;
        BtchFileDB &operator=(const BtchFileDB &) = delete;



        bool Init(const char* path, const char* ucPath);

        bool AddAddress(int type, const char *ip, int port);
        bool EnableAddress(int type, const char *ip, int port);
        bool DisableAddress(int type, const char *ip, int port);

        bool AddUnCheckedAddress(int type, const char *ip, int port);

        


    private:
        BtchFileDB();


        bool ReConnect(void** conn);

        void* Connect();

        bool Execute(void** conn, const char* sql);

    private:

        int             _fdAddress;
        int             _ucFdAddress;
   
};


#endif // __BTCH_DB_H__
