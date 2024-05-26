#include "stdio.h"
#include "defs.h"

const int BitTable[64] =
{
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};

int PopBit(U64 *bb) {
   U64 b = *bb ^ *bb - 1;
   unsigned int folded = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
   *bb &= (*bb - 1);
   return BitTable[(folded * 0x78291ACF) >> 26];
}
int CountBits(U64 b){
    int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;
}


void PrintBitBoard(U64 bb){
    
    U64 shiftMe = 1ULL;

    int rank = 0;
    int file = 0;
    int sq = 0;
    int sq64 = 0;

    printf("\n");
    for(rank = RANK_8; rank >= RANK_1; --rank){
        for(file = FILE_A; file <= FILE_H; ++file){
            sq = FR2SQ(file, rank);
            sq64 = SQ64(sq);
            if((shiftMe << sq64) & bb)
                printf("X");
            else 
                printf("-");
        }
        printf("\n");
    }
    printf("\n\n");
}