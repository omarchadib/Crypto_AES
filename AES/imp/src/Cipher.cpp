#include "Cipher.hpp"
#include "Transforms.hpp"

namespace aes {

Block encryptBlock(const Block& block, const RoundKeys& roundKeys,
                   std::vector<Block>* trace) {

    State state = toState(block);
    addRoundKey(state, roundKeys[0]);
    if(trace) trace->push_back(toBlock(state));

    //Tour 1 - 9
    for(int i=1; i<NumRounds; i++)
    {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state,roundKeys[i]);
        if(trace) trace->push_back(toBlock(state));
    }

    //Tour 10
    subBytes(state);
    shiftRows(state);
    addRoundKey(state,roundKeys[NumRounds]);
    if(trace) trace->push_back(toBlock(state));

    return toBlock(state);
}

}  // namespace aes