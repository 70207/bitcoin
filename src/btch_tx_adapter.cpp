#include <script/standard.h>
#include <base58.h>
#include <uint256.h>
#include <utilstrencodings.h>

#include <btch_tx_adapter.h>
#include <btch_tx_db.h>



int CreateTxIns(const CTransaction& tx, BtchTransaction& btx){
    BtchTransactionIn* ins = new BtchTransactionIn[btx.tx_in_count];
    for(int i = 0; i < btx.tx_in_count; ++i){
        BtchTransactionIn* p = ins + i;
        p->from_tx_hash = std::move(tx.vin[i].prevout.hash.GetHex());
        p->from_tx_vout = tx.vin[i].prevout.n;
        p->vin = i;
        p->sequence = tx.vin[i].nSequence;
        p->sig_hex = std::move(HexStr(tx.vin[i].scriptSig.begin(), tx.vin[i].scriptSig.end()));
    }

    btx.tx_in = ins;

    return 0;
}

int CreateTxOuts(const CTransaction& tx, BtchTransaction& btx){
    BtchTransactionOut* outs = new BtchTransactionOut[btx.tx_out_count];
    for(int i = 0; i < btx.tx_out_count; ++i){
        BtchTransactionOut* p = outs + i;
        p->vout = i;
        p->out_value = tx.vout[i].nValue;
        p->out_addr_count = 0;
        
        
        std::vector<CTxDestination> addresses;
        int nRequired;
        txnouttype type;
        if (!ExtractDestinations(tx.vout[i].scriptPubKey, type, addresses, nRequired)) {
            p->out_type = GetTxnOutputType(type);
            continue;
        }
        for (const CTxDestination& addr : addresses) {
            std::string sa = EncodeDestination(addr);
            p->out_addresses.append(sa);
            p->out_addresses.push_back(';');
            if(p->out_address.empty()){
                p->out_address = std::move(sa); 
            }
            p->out_addr_count++;
        }
       
    }

    btx.tx_out = outs;

    return 0;
}

int CreateTxIns(const CTransactionRef ptx, BtchTransaction& btx){
    BtchTransactionIn* ins = new BtchTransactionIn[btx.tx_in_count];
    for(int i = 0; i < btx.tx_in_count; ++i){
        BtchTransactionIn* p = ins + i;
        p->from_tx_hash = std::move(ptx->vin[i].prevout.hash.GetHex());
        p->from_tx_vout = ptx->vin[i].prevout.n;
        p->vin = i;
        p->sequence = ptx->vin[i].nSequence;
        p->sig_hex = std::move(HexStr(ptx->vin[i].scriptSig.begin(), ptx->vin[i].scriptSig.end()));
    }

    btx.tx_in = ins;

    return 0;
}

int CreateTxOuts(const CTransactionRef& ptx, BtchTransaction& btx){
    BtchTransactionOut* outs = new BtchTransactionOut[btx.tx_out_count];
    for(int i = 0; i < btx.tx_out_count; ++i){
        BtchTransactionOut* p = outs + i;
        p->vout = i;
        p->out_value = ptx->vout[i].nValue;
        p->out_addr_count = 0;
        
        
        std::vector<CTxDestination> addresses;
        int nRequired;
        txnouttype type;
        if (!ExtractDestinations(ptx->vout[i].scriptPubKey, type, addresses, nRequired)) {
            p->out_type = GetTxnOutputType(type);
            continue;
        }
        for (const CTxDestination& addr : addresses) {
            std::string sa = EncodeDestination(addr);
            p->out_addresses.append(sa);
            p->out_addresses.push_back(';');
            if(p->out_address.empty()){
                p->out_address = std::move(sa); 
            }
            p->out_addr_count++;
        }
       
    }

    btx.tx_out = outs;

    return 0;
}



int SaveTx2DB(const CTransaction& tx)
{
    std::string hash = std::move(tx.GetHash().GetHex());

    BtchTransaction    btx;
    btx.tx_hash = std::move(hash);
    btx.tx_in_count = tx.vin.size();
    btx.tx_out_count = tx.vout.size();

    if(btx.tx_in_count > 0){
        CreateTxIns(tx, btx);
    }


    if(btx.tx_out_count > 0){
        CreateTxOuts(tx, btx);
    }

    BtchTxDB::GetInstance()->AddTransaction(&btx);

    return 0;

}


// struct BtchBlock{
//     public:
//     BtchBlock();
//    ~BtchBlock();
    
//     std::string                 block_hash;
//     std::string                 pre_block_hash;
//     int                         version;
//     int                         time;
//     int                         bits;
//     int                         nonce;

//     int                         tx_count;
//     BtchTransaction*            tx;
//     BtchTransactionCoinBase*    coin_base;
// };


int SaveBlock2DB(const CBlock& block, int height)
{
    BtchBlock               bbk;

    bbk.block_hash = std::move(block.GetHash().GetHex());

    CBlockHeader  bh = block.GetBlockHeader();

    bbk.pre_block_hash = bh.hashPrevBlock.GetHex();
    bbk.version = bh.nVersion;
    bbk.time = bh.nTime;
    bbk.bits = bh.nBits;
    bbk.nonce = bh.nNonce;

    bbk.tx_count = block.vtx.size();
    
    
    if(bbk.tx_count <= 0){
        fprintf(stderr, "block tx count error, block hash:%s, count:%d\n", bbk.pre_block_hash.c_str(), bbk.tx_count);
        return -1;
    }

    bbk.tx = new BtchTransaction[bbk.tx_count];
    for(int i = 0; i < bbk.tx_count; ++i){
        BtchTransaction& btx = bbk.tx[i];
        const CTransactionRef& ptx = block.vtx[i];

        std::string hash = std::move(ptx->GetHash().GetHex());

        btx.tx_hash = std::move(hash);
        btx.tx_in_count = ptx->vin.size();
        btx.tx_out_count = ptx->vout.size();
        if(ptx->IsCoinBase()){
            btx.is_coinbase = 1;
        }
        else{
            btx.is_coinbase = 0;
        }
        

        if (btx.tx_in_count > 0) {
            CreateTxIns(ptx, btx);
        }


        if (btx.tx_out_count > 0) {
            CreateTxOuts(ptx, btx);
        }
    }

    BtchTxDB::GetInstance()->AddBlock(&bbk, height);

}