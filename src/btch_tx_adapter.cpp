#include <btch_tx_adapter.h>
#include <btch_tx_db.h>


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
    std::string     type;
    std::string     out_addresses;
    std::string     out_addr_count;
};


struct BtchTransaction{
    std::string             tx_hash;
    int                     tx_in_count;
    int                     tx_out_count;
    BtchTransactionIn*      tx_in;
    BtchTransactionOut*     tx_out;
};

int CreateTxIns(CTransaction& tx){
    BtchTransactionIn* ins = new BtchTransactionIn[btx.tx_in_count];
    for(int i = 0; i < btx.tx_in_count; ++i){
        BtchTransactionIn* p = ins[i];
        p->from_tx_hash = std::move(tx.vin[i].prevout.GetHex());
        p->vout = tx.vin[i].prevout.n;
    }
}

int CreateTxOuts(CTransaction& tx){
    BtchTransactionOut* outs = new BtchTransactionOut[btx.tx_out_count];
    for(int i = 0; i < btx.tx_out_count; ++i){
        BtchTransactionOut* p = outs[i];
        p->vout = i;
        p->out_value = tx.vout[i]->nValue;
        p->out_addr_count = 0;
        
        
        std::vector<CTxDestination> addresses;
        int nRequired;
        txnouttype type;
        if (!ExtractDestinations(tx.vout[i].scriptPubKey, type, addresses, nRequired)) {
            p->type = GetTxnOutputType(type);
            return;
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
}

int InsertTxDB(CTransaction& tx)
{
    std::string hash = std::move(tx.GetHash.GetHex());

    BtchTransaction    btx;
    btx.tx_hash = std::move(hash);
    btx.tx_in_count = tx.vin.size();
    btx.tx_out_count = tx.vout.size();

    if(btx.tx_in_count > 0){
        CreateTxIns(tx);
    }


    if(btx.tx_out_count > 0){
        CreateTxOuts(tx);
    }

    BtchTxDB::GetInstance()->AddTransaction(&tx);

}