#include "Transforms.hpp"
#include "algorithm"
#include "KeySchedule.hpp"


namespace aes{

// state[r][c] = block[4*c + r]
State toState(const Block& block)
{
    State state{};
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            state[i][j] = block[4*j + i];
        }
    }
    return state;
}


Block toBlock(const State& state)
{
    Block block{};
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            block[4*j + i] = state[i][j];
        }
    }
    return block;
}

void addRoundKey(State& state, const Key& roundKey)
{
    // XOR de l'état avec les 16 bytes de la sous clé
    // etat[r][c] ^= sous_cle[4*c + r]
    for (int r=0; r<4; r++)
    {
        for(int c=0; c<4; c++)
        {
            state[r][c] ^= roundKey[4*c + r];
        }
    }
    return;
}

void subBytes(State& state) 
{
    for (auto& row : state) {
        for (Byte& b : row) {
            b = SBox[b];
        }
    }
}

// Ligne r decalee vers la gauche de r positions
void shiftRows(State& state) 
{
    for (std::size_t r = 1; r < 4; ++r) {
        std::rotate(state[r].begin(), state[r].begin() + r, state[r].end());
    }
}

Byte xtime(Byte a) {
    return static_cast<Byte>((a << 1) ^ (0x1B * (a >> 7)));
}

void mixColumns(State& state)
{
    for (std::size_t c = 0; c < 4; ++c) {
        const Byte a0 = state[0][c];
        const Byte a1 = state[1][c];
        const Byte a2 = state[2][c];
        const Byte a3 = state[3][c];

        // 3*x = xtime(x) ^ x
        state[0][c] = xtime(a0) ^ (xtime(a1) ^ a1) ^ a2 ^ a3;
        state[1][c] = a0 ^ xtime(a1) ^ (xtime(a2) ^ a2) ^ a3;
        state[2][c] = a0 ^ a1 ^ xtime(a2) ^ (xtime(a3) ^ a3);
        state[3][c] = (xtime(a0) ^ a0) ^ a1 ^ a2 ^ xtime(a3);
    }
}

}