
#include "KeySchedule.hpp"

namespace aes
{

using Word = std::array<Byte, WordSize>;

// Rotation circulaire d'un mot vers la gauche
Word rotWord(const Word& w)
{
    return {w[1], w[2], w[3], w[0]};
}

// Substitution de chaque octet avec la S-Box
Word subWord(const Word& w)
{
    return {
        SBox[w[0]],
        SBox[w[1]],
        SBox[w[2]],
        SBox[w[3]]
    };
}

// Expansion de la cle initiale en cles de tour
RoundKeys expandKey(const Key& key)
{
    // Tableau contenant les 44 mots de l'expansion AES-128
    std::array<Word, NumWords> words{};

    // 1. Copie des 4 mots de la cle initiale
    for (std::size_t i = 0; i < NumKeyWords; ++i)
    {
        for (std::size_t j = 0; j < WordSize; ++j)
        {
            words[i][j] = key[WordSize * i + j];
        }
    }

    // 2. Generation des mots suivants
    for (std::size_t i = NumKeyWords; i < NumWords; ++i)
    {
        Word temp = words[i - 1];

        if (i % NumKeyWords == 0)
        {
            temp = subWord(rotWord(temp));
            temp[0] ^= Rcon[i / NumKeyWords - 1];
        }

        for (std::size_t j = 0; j < WordSize; ++j)
        {
            words[i][j] = words[i - NumKeyWords][j] ^ temp[j];
        }
    }

    // 3. Regroupement des 44 mots en 11 cles de tour
    RoundKeys roundKeys{};

    for (std::size_t r = 0; r < NumRoundKeys; ++r)
    {
        for (std::size_t w = 0; w < NumKeyWords; ++w)
        {
            for (std::size_t j = 0; j < WordSize; ++j)
            {
                roundKeys[r][WordSize * w + j] =
                    words[NumKeyWords * r + w][j];
            }
        }
    }

    return roundKeys;
}

}
