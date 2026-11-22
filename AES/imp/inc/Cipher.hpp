#ifndef CIPHER_HPP
#define CIPHER_HPP

#include <vector>

#include "Types.hpp"
#include "Tables.hpp"

namespace aes {

// Equivalent de la fonctiokn chiffrer_bloc
Block encryptBlock(const Block& block, const RoundKeys& roundKeys,
                   std::vector<Block>* trace = nullptr);

}  // namespace aes

#endif  // CIPHER_HPP