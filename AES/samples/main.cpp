
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <string>

#include "Transforms.hpp"
#include "Cipher.hpp"
#include "KeySchedule.hpp"

using namespace aes;

// Afficher un bloc de 16 octets en hexadecimal
void afficherBloc(const Block& block)
{
    for (const auto& octet : block)
    {
        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(octet);
    }
}

// Comparer un bloc obtenu avec le bloc de reference NIST
bool verifierBloc(
    const std::string& label,
    const Block& obtenu,
    const Block& attendu)
{
    bool correct = (obtenu == attendu);

    std::cout << std::left << std::setw(25) << label;

    afficherBloc(obtenu);

    std::cout << "  " << (correct ? "[OK]" : "[ECHEC]") << '\n';

    // En cas d'erreur, afficher la valeur attendue
    if (!correct)
    {
        std::cout << std::setw(25) << "Valeur attendue :";
        afficherBloc(attendu);
        std::cout << '\n';
    }

    return correct;
}

int main()
{
    std::cout << "========== TEST AES-128 NIST ==========\n\n";

    // 1. Initialisation du texte en clair (Plaintext)
    Block block = {
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB,
        0xCC, 0xDD, 0xEE, 0xFF
    };

    // 2. Initialisation de la cle AES-128
    Key key = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F
    };

    // 3. Valeurs de reference NIST apres chaque tour
    // trace[0] correspond a l'etat apres AddRoundKey initial
    // trace[1] correspond a la fin du tour 1
    // ...
    // trace[10] correspond a la fin du tour 10

    const std::array<Block, NumRoundKeys> references = {{

        // Etat apres AddRoundKey initial (ARK0)
        {0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,
         0x80,0x90,0xA0,0xB0,0xC0,0xD0,0xE0,0xF0},

        // Fin du tour 1
        {0x89,0xD8,0x10,0xE8,0x85,0x5A,0xCE,0x68,
         0x2D,0x18,0x43,0xD8,0xCB,0x12,0x8F,0xE4},

        // Fin du tour 2
        {0x49,0x15,0x59,0x8F,0x55,0xE5,0xD7,0xA0,
         0xDA,0xCA,0x94,0xFA,0x1F,0x0A,0x63,0xF7},

        // Fin du tour 3
        {0xFA,0x63,0x6A,0x28,0x25,0xB3,0x39,0xC9,
         0x40,0x66,0x8A,0x31,0x57,0x24,0x4D,0x17},

        // Fin du tour 4
        {0x24,0x72,0x40,0x23,0x69,0x66,0xB3,0xFA,
         0x6E,0xD2,0x75,0x32,0x88,0x42,0x5B,0x6C},

        // Fin du tour 5
        {0xC8,0x16,0x77,0xBC,0x9B,0x7A,0xC9,0x3B,
         0x25,0x02,0x79,0x92,0xB0,0x26,0x19,0x96},

        // Fin du tour 6
        {0xC6,0x2F,0xE1,0x09,0xF7,0x5E,0xED,0xC3,
         0xCC,0x79,0x39,0x5D,0x84,0xF9,0xCF,0x5D},

        // Fin du tour 7
        {0xD1,0x87,0x6C,0x0F,0x79,0xC4,0x30,0x0A,
         0xB4,0x55,0x94,0xAD,0xD6,0x6F,0xF4,0x1F},

        // Fin du tour 8
        {0xFD,0xE3,0xBA,0xD2,0x05,0xE5,0xD0,0xD7,
         0x35,0x47,0x96,0x4E,0xF1,0xFE,0x37,0xF1},

        // Fin du tour 9
        {0xBD,0x6E,0x7C,0x3D,0xF2,0xB5,0x77,0x9E,
         0x0B,0x61,0x21,0x6E,0x8B,0x10,0xB6,0x89},

        // Fin du tour 10 : texte chiffre final
        {0x69,0xC4,0xE0,0xD8,0x6A,0x7B,0x04,0x30,
         0xD8,0xCD,0xB7,0x80,0x70,0xB4,0xC5,0x5A}
    }};

    // 4. Expansion de la cle initiale en cles de tour
    RoundKeys roundKeys = expandKey(key);

    // 5. Chiffrement du bloc et recuperation des etats intermediaires
    std::vector<Block> trace;

    Block encrypted = encryptBlock(block, roundKeys, &trace);

    // 6. Affichage du texte en clair et de la cle
    std::cout << "TEXTE EN CLAIR : ";
    afficherBloc(block);

    std::cout << "\nCLE AES        : ";
    afficherBloc(key);

    std::cout << "\n\n========== VERIFICATION DES TOURS ==========\n\n";

    // 7. Verification du nombre d'etats enregistres
    if (trace.size() != NumRoundKeys)
    {
        std::cerr << "ERREUR : " << NumRoundKeys
                  << " etats etaient attendus, mais "
                  << trace.size() << " ont ete enregistres.\n";

        return 1;
    }

    bool tousCorrects = true;

    // 8. Comparaison des etats intermediaires avec les references NIST
    for (std::size_t i = 0; i < trace.size(); ++i)
    {
        std::string label;

        if (i == 0)
            label = "Apres ARK initial :";
        else
            label = "Fin du tour " + std::to_string(i) + " :";

        if (!verifierBloc(label, trace[i], references[i]))    // ON compare les trace avce les ref du NIST
            tousCorrects = false;
    }

    // 9. Verification du texte chiffre final
    std::cout << "\n========== RESULTAT DU CHIFFREMENT ==========\n\n";

    const Block chiffreAttendu = {
        0x69,0xC4,0xE0,0xD8,
        0x6A,0x7B,0x04,0x30,
        0xD8,0xCD,0xB7,0x80,
        0x70,0xB4,0xC5,0x5A
    };

    bool chiffrementCorrect = verifierBloc(
        "Texte chiffre obtenu :",
        encrypted,
        chiffreAttendu
    );

    // 10. Bilan global des tests
    std::cout << "\n========== BILAN DES TESTS ==========\n";

    std::cout << "Etats intermediaires : "
              << (tousCorrects ? "REUSSITE" : "ECHEC") << '\n';

    std::cout << "Chiffrement final    : "
              << (chiffrementCorrect ? "REUSSITE" : "ECHEC") << '\n';

    std::cout << "Resultat global      : "
              << ((tousCorrects && chiffrementCorrect)
                  ? "SUCCES"
                  : "ECHEC") << '\n';

    std::cout << "=====================================\n";

    return (tousCorrects && chiffrementCorrect) ? 0 : 1;
}
