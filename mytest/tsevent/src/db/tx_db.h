// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __BTCH_TX_DB_H__
#define __BTCH_TX_DB_H__


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
    int                     tx_out_count;
    BtchTransactionIn*      tx_in;
    BtchTransactionOut*     tx_out;
};

struct BtchBlock{
    std::string                 block_hash;
    int                         tx_count;
    BtchTransaction*            tx;
    BtchTransactionCoinBase*    coin_base;
};


class BtchTxDB 
{
    public:
        static BtchTxDB* GetInstance();

        BtchTxDB(const BtchTxDB &) = delete;
        BtchTxDB &operator=(const BtchTxDB &) = delete;



        bool Init(const char* db = "bitcoin");
        bool AddBlock(BtchBlock* block);
        bool AddTransaction(BtchTransaction* tx);

    private:
        bool AddBlockTransaction(BtchTransaction* tx, long block_id, int index);
        bool AddBlockTransactionCoinBase(BtchTransactionCoinBase* tx, long block_id);


        bool AddTxTransaction(BtchTransaction* tx);

        bool AddTxTransactionIn(void** conn, BtchTransactionIn* tx_in, long tx_id, int index);
        bool AddTxTransactionOut(void** conn, BtchTransactionOut* tx_out, long tx_id, int index);

    private:
        BtchTxDB();


        bool ReConnect(void** conn);

        void* Connect();

        bool Execute(void** conn, const char* sql);

        long Insert(void** conn, const char* sql);

    private:

        void*      _blockConn;
        void*      _txConn;

        const char* _db;



};


#endif // __BTCH_DB_H__
