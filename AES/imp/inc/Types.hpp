#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <array>
#include <cstddef>
#include <cstdint>


namespace aes {

using Byte = std::uint8_t;

inline constexpr std::size_t BlockSize    = 16;                         // bytes
inline constexpr std::size_t NumRounds    = 10;                         // 10 tours
inline constexpr std::size_t NumRoundKeys = NumRounds + 1;              // Nbr de sous clés
inline constexpr std::size_t WordSize     = 4;
inline constexpr std::size_t NumWords     = WordSize*NumRoundKeys;      // nbr de mots
inline constexpr std::size_t NumKeyWords  = BlockSize/WordSize;         //nbr de mots par clés = 4

using Block     = std::array<Byte, BlockSize>;
using Key       = std::array<Byte, BlockSize>;
using RoundKeys = std::array<std::array<Byte, BlockSize>, NumRoundKeys>; // Les sous_clés de 16bytes = 4 mots
using State     = std::array<std::array<Byte,4>, 4>;

} // namespace aes

#endif
