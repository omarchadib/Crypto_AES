#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "Types.hpp"
#include "Tables.hpp"

namespace aes {

State toState(const Block& block);
Block toBlock(const State& state);

void addRoundKey(State& state, const Key& roundKey);
void subBytes(State& state);
void shiftRows(State& state);
Byte xtime(Byte a);
void mixColumns(State& state);

}  // namespace aes

#endif  // TRANSFORMS_HPP