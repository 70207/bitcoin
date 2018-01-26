// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __BTCH_DB_H__
#define __BTCH_DB_H__


#include <string>

struct BtchTransactionCoinBase{
    std::string     tx_hash;
    std::string     coinbase_hash;
    long            coinbase_sequence;
    long            coinbase_value;
    std::string     out_address;
};


struct BtchTransactionIn{
    std::string     from_tx_hash;
    long            vout;
};

struct BtchTransactionOut{
    long            vout;
    long            out_value;
    std::string     out_address;
};


struct BtchTransaction{
    std::string             tx_hash;
    int                     tx_in_count;
    int                     tx_out_out;
    BtchTransactionIn*      tx_in;
    BtchTransactionOut*     tx_out;
};

struct BtchBlock{
    std::string                 block_hash;
    int                         tx_count;
    BtchTransaction*            tx;
    BtchTransactionCoinBase*    coin_base;
};


class BtchDB 
{
    public:
        static BtchDB* GetInstance();

        BtchDB(const BtchDB &) = delete;
        BtchDB &operator=(const BtchDB &) = delete;



        bool Init(const char* db = "bitcoin", const char* table = "address", const char* ucTable = "ucAddress");

        bool AddAddress(int type, const char *ip, int port);
        bool EnableAddress(int type, const char *ip, int port);
        bool DisableAddress(int type, const char *ip, int port);

        bool AddUnCheckedAddress(int type, const char *ip, int port);


        bool AddBlock(BtchBlock* block);
        bool AddTransaction(BtchTransaction* tx);

    private:
        bool AddBlockTransaction(BtchTransaction* tx, long block_id);
        bool AddBlockTransactionCoinBase(BtchTransactionCoinBase* tx, long block_id);


        bool AddTxTransaction(BtchTransaction* tx);

        bool AddTxTransactionIn(void** conn, BtchTransactionIn* tx_in, long tx_id, int index);
        bool AddTxTransactionOut(void** conn, BtchTransactionOut* tx_out, long tx_id, int index);

    private:
        BtchDB();


        bool ReConnect(void** conn);

        void* Connect();

        bool Execute(void** conn, const char* sql);

        long Insert(void** conn, const char* sql);

    private:
        void*      _addConn;
        void*      _enableConn;
        void*      _disableConn;

        void*      _ucAddConn;

        void*      _blockConn;
        void*      _txConn;

        const char* _db;
        const char* _table;
        const char* _ucTable;


};


#endif // __BTCH_DB_H__
