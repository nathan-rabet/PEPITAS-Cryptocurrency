#include "core/blockchain/block.h"

ChunkBlockchain get_blockchain(size_t nb_chunk) {
    static ChunkBlockchain blockchain = {0};

    if (nb_chunk == 0 && blockchain.chunk_nb != 0)
        return blockchain;
    
    // TODO load blockchain
    return blockchain;
}
