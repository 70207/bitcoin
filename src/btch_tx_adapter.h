#ifndef __BTCH_TX_ADAPTER__
#define __BTCH_TX_ADAPTER__

#include <primitives/transaction.h>
#include <primitives/block.h>

int SaveTx2DB(const CTransaction& tx);

int SaveBlock2DB(const CBlock& block, int height);


#endif