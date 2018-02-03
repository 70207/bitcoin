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
    std::string     coinbase_hash;
    std::string     sig_hex;
    long            sequence;
    int             from_tx_vout;
    int             vin;
};

struct BtchTransactionOut{
    long            vout;
    long            out_value;
    std::string     out_address;
    std::string     out_type;
    std::string     out_addresses;
    int             out_addr_count;
};

struct BtchTransaction{
    public:
    BtchTransaction();
    ~BtchTransaction();

    long                        tx_id; //for memory
    std::string                 tx_hash;
    int                         tx_time;
    int                         tx_in_count;
    int                         tx_out_count;
    BtchTransactionIn*          tx_in;
    BtchTransactionOut*         tx_out;
    int                         is_coinbase;
};

// create table block(
//     `id` bigint auto_increment not null,
//     `block_hash` varchar(128) not null,
//     `pre_block_hash` varchar(128) not null, 
//     `version` int not null, 
//     `time` int not null,
//     `bits` int not null,
//     `nonce` int not null,

//     `status` tinyint(3) not null,
//     primary key (`id`),
//     unique key(`block_hash`)
// )engine=myisam;

struct BtchBlock{
    public:
    BtchBlock();
   ~BtchBlock();
    
    std::string                 block_hash;
    std::string                 pre_block_hash;
    int                         version;
    unsigned int                time;
    unsigned int                bits;
    unsigned int                nonce;

    int                         tx_count;
    BtchTransaction*            tx;
};


class BtchTxDB 
{
    public:
        static BtchTxDB* GetInstance();

        BtchTxDB(const BtchTxDB &) = delete;
        BtchTxDB &operator=(const BtchTxDB &) = delete;



        bool Init(const char* db = "bitcointx");
        bool AddBlock(BtchBlock* block, int height);
        bool AddTransaction(BtchTransaction* tx);

    private:
        bool AddBlockTransaction(BtchTransaction* tx, long block_id, int index);
        bool AddBlockTransactionCoinBase(BtchTransactionCoinBase* tx, long block_id);


        bool AddTxTransaction(BtchTransaction* tx);

        bool AddTxTransactionIn(void** conn, BtchTransactionIn* tx_in, BtchTransaction* tx);
        bool AddTxTransactionOut(void** conn, BtchTransactionOut* tx_out, BtchTransaction* tx);
        bool CheckTxTransactionUXTO(void** conn, BtchTransactionIn* tx_in, BtchTransaction* tx);

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
