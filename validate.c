
#include "defs.h"

// Checks if a square is on board
int SqOnBoard(const int sq) {
    return FilesBrd[sq] == OFFBOARD? 0 : 1;
}

int SideValid(const int side) {
    return (side == WHITE || side == BLACK) ? 1 : 0;
}

int FileRankValid(const int fr) {
    return (fr >= 0 && fr <= 7) ? 1 : 0;
}

int PceValidEmptyOffbrd(const int pce) {
	return (PieceValidEmpty(pce) || pce == OFFBOARD);
}

int PieceValidEmpty(const int pce) {
    return (pce >= EMPTY && pce <= bK /* or 12 */) ? 1 : 0;
}

int PieceValid(const int pce) {
    return (pce >= wP && pce <= bK) ? 1 : 0;
}


