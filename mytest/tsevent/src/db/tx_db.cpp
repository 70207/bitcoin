
#include <stdio.h>
#include <mysql.h>
#include <errmsg.h>
#include <tx_db.h>


BtchTransaction::BtchTransaction()
:tx_in_count(0), tx_out_count(0)
,tx_in(0), tx_out(0),is_coinbase(0)
{

}

BtchTransaction::~BtchTransaction()
{
    if(tx_in){
        delete[] tx_in;
    }

    if(tx_out){
        delete[] tx_out;
    }
}


BtchBlock::BtchBlock()
:tx_count(0),tx(0)
{

}

BtchBlock::~BtchBlock()
{
    if(tx){
        delete[] tx;
    }

}


static BtchTxDB* btch_tx_db = 0;

BtchTxDB* BtchTxDB::GetInstance()
{
    if(!btch_tx_db){
        btch_tx_db = new BtchTxDB();
    }

    return btch_tx_db;
}

BtchTxDB::BtchTxDB():
_blockConn(0),
_txConn(0)
{

	
}

bool BtchTxDB::Init(const char* db)
{
    _db = db;
    _blockConn = Connect();
    _txConn = Connect();

}


bool BtchTxDB::ReConnect(void** conn)
{
    if(*conn != 0){
         MYSQL *c = (MYSQL*)(*conn);
         mysql_close(c);
    }

    *conn = 0;

    void* c = Connect();
    if(!c){
        return false;
    }

    *conn = c;
    return true;   
}

void* BtchTxDB::Connect()
{
    MYSQL *conn;
    MYSQL_RES *res;
	MYSQL_ROW row;
	const char *server = "127.0.0.1";
	const char *user = "root";
	const char *password = "sd-9898w";  
	const char *database = _db;//"bitcoin";
	conn = mysql_init(NULL);


    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
        return 0;
	}

    return conn;
}



bool BtchTxDB::Execute(void** conn, const char* sql)
{
    if(*conn == 0){
        if(!ReConnect(conn)){
            return false;
        }
    }
    
    bool toBreak = false;
    while (!toBreak)
    {
        MYSQL* c = (MYSQL*)(*conn);
        if (mysql_query(c, sql))
        {
            int err = mysql_errno(c);
            switch (err)
            {
                case CR_SERVER_GONE_ERROR:
                case CR_SERVER_LOST:
                    {
                         if(ReConnect(conn)){
                             continue;
                         }   
                   }
            }

            fprintf(stderr, "%s\n", mysql_error(c));
            return false;
        }
        else{
            return true;
        }
    }

    return true;
}


long BtchTxDB::Insert(void** conn, const char* sql)
{
    if(*conn == 0){
        if(!ReConnect(conn)){
            return false;
        }
    }
    
    bool toBreak = false;
    while (!toBreak)
    {
        MYSQL* c = (MYSQL*)(*conn);
        if (mysql_query(c, sql))
        {
            int err = mysql_errno(c);
            switch (err)
            {
                case CR_SERVER_GONE_ERROR:
                case CR_SERVER_LOST: 
                    {
                         if(ReConnect(conn)){
                             continue;
                         }   
                   }
            }

            fprintf(stderr, "%s\n", mysql_error(c));
            return 0;
        }
        else{
            return mysql_insert_id(c);
        }
    }

    return 0;
}

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


bool BtchTxDB::AddBlock(BtchBlock* block, int height)
{
    fprintf(stderr,"add block, block hash:%s\n", block->block_hash.c_str());

    char buffer[4096];
    const char* sql = "insert into block(`height`, `block_hash`, `pre_block_hash`,`version`, `time`, `bits`, `nonce`,\
                    `tx_count`,`status`) values(%d, '%s', '%s', %u, %u, %u, %u,%d,\
                    2) on duplicate key update `id` = LAST_INSERT_ID(`id`), `status` = 2";

    sprintf(buffer, sql,  height, block->block_hash.c_str(), block->pre_block_hash.c_str(),
    block->version, block->time, block->bits, block->nonce, block->tx_count);

   // printf("sql:%s\n", buffer);

    long block_id = Insert(&_blockConn, buffer);
    if(!block_id){
        fprintf(stderr, "insert block failed, nonce:%u, block hash:%s\n", block->nonce, block->block_hash.c_str());
        return false;
    }
    

    for(int i = 0; i < block->tx_count; ++i){
        BtchTransaction* tx = block->tx + i;
        bool flag = AddBlockTransaction(tx, block_id, i);
        if(!flag){
            fprintf(stderr, "insert transaction failed, tx hash:%s\n", tx->tx_hash.c_str());
            return false;
        }
    }

    // if(block->coin_base){
    //     return AddBlockTransactionCoinBase(block->coin_base, block_id);
    // }

    return true;

}

bool BtchTxDB::AddTransaction(BtchTransaction* tx)
{
    return AddTxTransaction(tx);
}

// create table tx(
//     id bigint auto_increment not null,
//     tx_hash varchar(128) not null,
//     block_id bigint not null,
//     is_coinbase tinyint(3) not null,
//     status tinyint(3) not null,
//     primary key (`id`),
//     unique key(`tx_hash`),
//     CONSTRAINT block_id FOREIGN KEY (block_id) REFERENCES block (id)
// )engine=myisam;





// struct BtchTransaction{
//     std::string             tx_hash;
//     int                     tx_in_count;
//     int                     tx_out_out;
//     BtchTransactionIn*      tx_in;
//     BtchTransactionOut*     tx_out;
// };


bool BtchTxDB::AddBlockTransaction(BtchTransaction* tx, long block_id, int index)
{
    fprintf(stderr,"add block trasaction, block id:%ld, tx hash:%s\n", block_id, tx->tx_hash.c_str());
    char buffer[40960];
    const char* sql = "insert into tx(`tx_hash`, `block_id`, `indexof_block`, `is_coinbase`, \
                    `tx_in_count`, `tx_out_count`,`status`) values('%s', %ld, %d, %d,\
                    %d, %d,\
                    2) on duplicate key update  `id` = LAST_INSERT_ID(`id`), `status` = 2";
    sprintf(buffer, sql, tx->tx_hash.c_str(), block_id, index, tx->is_coinbase, 
        tx->tx_in_count, tx->tx_out_count);

    long tx_id = Insert(&_blockConn, buffer);
    if (!tx_id)
    {
        fprintf(stderr, "insert tx failed, block id:%ld, tx hash:%s\n", block_id, tx->tx_hash.c_str());
        return false;
    }

    tx->tx_id = tx_id;
    for (int i = 0; i < tx->tx_in_count; ++i)
    {
        if (!AddTxTransactionIn(&_blockConn, tx->tx_in + i, tx))
        {
            fprintf(stderr, "insert block tx in failed, block id:%ld, tx hash:%s, i:%d\n", block_id, tx->tx_hash.c_str(), i);
            return false;
        }

        if(!CheckTxTransactionUXTO(&_blockConn, tx->tx_in + i, tx)){
            fprintf(stderr, "check block tx in failed, tx hash:%s, i:%d\n",tx->tx_hash.c_str(), i);
            return false;
        }
    }

    for (int i = 0; i < tx->tx_out_count; ++i)
    {
        if (!AddTxTransactionOut(&_txConn, tx->tx_out + i, tx))
        {
            fprintf(stderr, "insert block tx out failed, block id:%ld, tx hash:%s, i:%d\n", block_id, tx->tx_hash.c_str(), i);
            return false;
        }
    }

    return true;
}


bool BtchTxDB::AddTxTransaction(BtchTransaction* tx)
{
    fprintf(stderr,"add tx trasaction, tx hash:%s\n", tx->tx_hash.c_str());
    char buffer[40960];
    const char* sql = "insert into tx(`tx_hash`, `is_coinbase`, `status`) values('%s', 0, 1) \
        on duplicate key update  `id` = LAST_INSERT_ID(`id`), `status` = `status`";
    sprintf(buffer, sql, tx->tx_hash.c_str());

    long tx_id = Insert(&_txConn, buffer);
    if (!tx_id)
    {
        fprintf(stderr, "insert tx tx failed, tx hash:%s\n", tx->tx_hash.c_str());
        return false;
    }

    for (int i = 0; i < tx->tx_in_count; ++i)
    {
        if (!AddTxTransactionIn(&_txConn, tx->tx_in + i, tx))
        {
            fprintf(stderr, "insert tx tx in failed, tx hash:%s, i:%d\n",tx->tx_hash.c_str(), i);
            return false;
        }

        if(!CheckTxTransactionUXTO(&_txConn, tx->tx_in + i, tx)){
            fprintf(stderr, "check tx tx in failed, tx hash:%s, i:%d\n",tx->tx_hash.c_str(), i);
            return false;
        }
    }

    for (int i = 0; i < tx->tx_out_count; ++i)
    {
        if (!AddTxTransactionOut(&_txConn, tx->tx_out + i, tx))
        {
            fprintf(stderr, "insert tx tx out failed, tx hash:%s, i:%d\n",tx->tx_hash.c_str(), i);
            return false;
        }
    }

    return true;
}

// struct BtchTransactionIn{
//     std::string     from_tx_hash;
//     long            vout;
// };



// create table tx_in(
//     id bigint auto_increment not null,
//     tx_id bigint not null,
//     from_tx_hash varchar(65) not null,
//     vout int not null,
//     primary key(id),
//     KEY tx_id (tx_id),
//     CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
// )engine=myisam;




// bool BtchTxDB::AddTxTransactionIn(void** conn, BtchTransactionIn* tx_in, BtchTransaction* tx)
// {
//     fprintf(stderr,"add tx_in trasaction, tx_id:%ld, index:%d\n", tx->tx_id, tx_in->vin);
//     fprintf(stderr, "tx in sig hex:%s\n", tx_in->sig_hex.c_str());

//     char buffer[40960];
//     const char* sql = "insert into tx_in(`tx_id`, `tx_hash`, `vin`, \
//                     `from_tx_hash`, `from_tx_vout`, \
//                     `sig_hex`, `sequence`,\
//                   `status`) values(%ld, '%s', %d, \
//                    '%s', %d, \
//                     '%s', %ld, 1) \
//                   on duplicate key update `id` = LAST_INSERT_ID(`id`),`status` = `status`";


//     sprintf(buffer, sql, 
//     tx->tx_id, tx->tx_hash.c_str(), tx_in->vin, 
//     tx_in->from_tx_hash.c_str(),  tx_in->from_tx_vout,
//     tx_in->sig_hex.c_str(), tx_in->sequence);
//     bool ret = Insert(conn, buffer) > 0;
//     if(!ret){
//         fprintf(stderr, "add tx_in failed, from tx out:%d\n", tx_in->from_tx_vout);
//     }

//     return ret;
// }

bool BtchTxDB::AddTxTransactionIn(void** conn, BtchTransactionIn* tx_in, BtchTransaction* tx)
{
    fprintf(stderr,"add tx_in trasaction, tx_id:%ld, index:%d\n", tx->tx_id, tx_in->vin);
    fprintf(stderr, "tx in sig hex:%s\n", tx_in->sig_hex.c_str());

    char buffer[40960];
    const char* sql = "insert into tx_in(`tx_id`, `tx_hash`, `vin`, \
                    `from_tx_hash`, `from_tx_vout`, `sequence`,\
                  `status`) values(%ld, '%s', %d, \
                   '%s', %d, %ld, 1) \
                  on duplicate key update `id` = LAST_INSERT_ID(`id`),`status` = `status`";


    sprintf(buffer, sql, 
    tx->tx_id, tx->tx_hash.c_str(), tx_in->vin, 
    tx_in->from_tx_hash.c_str(),  tx_in->from_tx_vout, tx_in->sequence);
    bool ret = Insert(conn, buffer) > 0;
    if(!ret){
        fprintf(stderr, "add tx_in failed, from tx out:%d\n", tx_in->from_tx_vout);
    }

    return ret;
}

bool BtchTxDB::CheckTxTransactionUXTO(void** conn, BtchTransactionIn* tx_in, BtchTransaction* tx)
{
    fprintf(stderr,"check uxto trasaction, tx_id:%ld, from_vout:%d, vout:%d\n", tx->tx_id, tx_in->from_tx_vout, tx_in->vin);
    char buffer[40960];
    const char* sql = "update tx_out set `status` = 3 where `tx_hash` = '%s' and `vout` = %d";


    sprintf(buffer, sql, tx_in->from_tx_hash.c_str(), tx_in->from_tx_vout);
    return Execute(conn, buffer);
}

// struct BtchTransactionOut{
//     long            vout;
//     long            out_value;
//     std::string     out_address;
// };

// create table tx_out(
//     id bigint auto_increment not null,
//     tx_id bigint not null,
//     vout int not null,
//     out_value bigint not null,
//     out_address varchar(64) not null,
//     primary key(id),
//     KEY tx_id (tx_id),
//     CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
// )engine=myisam;

bool BtchTxDB::AddTxTransactionOut(void** conn, BtchTransactionOut* tx_out, BtchTransaction* tx)
{
    fprintf(stderr,"add tx_out trasaction, tx_id:%ld, index:%ld\n", tx->tx_id, tx_out->vout);
    char buffer[40960];
    const char* sql = "insert into tx_out(`tx_id`, `tx_hash`, `vout`, `out_value`,`out_address`,\
                            `out_type`,`out_addresses`,`out_addr_count`,`status`) values( \
                                %ld, '%s', %ld, %ld, '%s', \
                                '%s', '%s',%d, \
                                1) on duplicate key update `id` = LAST_INSERT_ID(`id`),`status` = `status`";

    sprintf(buffer, sql, tx->tx_id,  tx->tx_hash.c_str(),  tx_out->vout, tx_out->out_value, tx_out->out_address.c_str(),
        tx_out->out_type.c_str(), tx_out->out_addresses.c_str(), tx_out->out_addr_count);
    return Insert(conn, buffer) > 0;
}

// create table tx(
//     id bigint auto_increment not null,
//     tx_hash varchar(128) not null,
//     block_id bigint not null,
//     is_coinbase tinyint(3) not null,
//     status tinyint(3) not null,
//     primary key (`id`),
//     unique key(`tx_hash`),
//     CONSTRAINT block_id FOREIGN KEY (block_id) REFERENCES block (id)
// )engine=myisam;

// create table tx_coinbase(
//     id bigint auto_increment not null,
//     tx_id bigint not null,
//     coinbase_hash varchar(128),
//     coinbase_sequence bigint,
//     coinbase_value bigint not null,
//     out_address varchar(64) not null,
//     primary key(id),
//     KEY tx_id (tx_id),
//     CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
// )engine=myisam;

bool BtchTxDB::AddBlockTransactionCoinBase(BtchTransactionCoinBase* tx, long block_id)
{
    fprintf(stderr,"add tx coinbase, block_id:%ld, tx hash:%s\n", block_id, tx->tx_hash.c_str());
    char buffer[40960];
    const char* sql = "insert into tx(`tx_hash`, `block_id`, `is_coinbase`, `status`) values('%s',\
                %ld, 1, 2) on duplicate key update `id` = LAST_INSERT_ID(`id`), status = 2";
    sprintf(buffer, sql, tx->tx_hash.c_str(), block_id);


    long tx_id = Insert(&_blockConn, buffer);
    if(!tx_id){
        fprintf(stderr, "insert coinbase failed, block id:%ld, tx hash:%s\n", block_id, tx->tx_hash.c_str());
        return false;
    }

    sql = "insert into tx_coinbase(`tx_id`, `coinbase_hash`, `coinbase_sequence`, `coinbase_value`, `out_address`, `status`) values(%d,\
                '%s', %ld, %ld, '%s', 2) on duplicate key update `id` = LAST_INSERT_ID(`id`), `status` = 2";

    sprintf(buffer, sql, tx_id, tx->coinbase_hash.c_str(), tx->coinbase_sequence, tx->coinbase_value, tx->out_address.c_str());

    long cb_id = Insert(&_blockConn, buffer);
    return cb_id > 0;
}