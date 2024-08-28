// makemove.c

#include "defs.h"
#include "stdio.h"

#define HASH_PCE(pce, sq) (pos->posKey ^= (PieceKeys[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (CastleKeys[(pos->castlePerm)]))
#define HASH_SIDE (pos->posKey ^= (SideKey))
#define HASH_EP (pos->posKey ^= (PieceKeys[EMPTY][(pos->enPas)]))

const int CastlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

// Removing a piece from the board
static void ClearPiece(const int sq, S_BOARD *pos) {

    ASSERT(SqOnBoard(sq));

    int pce = pos->pieces[sq];

    ASSERT(PieceValid(pce));

    int col = PieceCol[pce];
    int index = 0;
    int t_pcenum = -1;

    // remove from hash
    HASH_PCE(pce, sq);

    pos->pieces[sq] = EMPTY;
    pos->material[col] -= PieceVal[pce];

    if(PieceBig[pce]) {
        pos->bigPce[col]--;
        if(PieceMaj[pce]) {
            pos->majPce[col]--;
        } else {
            pos->minPce[col]--;
        }
    } else {
        CLRBIT(pos->pawns[col], SQ64(sq));
        CLRBIT(pos->pawns[BOTH], SQ64(sq));
    }

    
    // Loop through all pieces in the board of specific
    // kind and store the index
    for(index = 0; index < pos->pceNum[pce]; index++) {
        if(pos->pList[pce][index] == sq) {
            t_pcenum = index;
            break;
        }
    }

    ASSERT(t_pcenum != -1);

    pos->pceNum[pce]--;
    pos->pList[pce][t_pcenum] = pos->pList[pce][pos->pceNum[pce]];
}

// Adding a piece to the board
static void AddPiece(int sq, S_BOARD *pos, int pce) {

    ASSERT(PieceValid(pce));
    ASSERT(SqOnBoard(sq));

    int col = PieceCol[pce];

    HASH_PCE(pce, sq);

    pos->pieces[sq] = pce;

    if(PieceBig[pce]) {
        pos->bigPce[col]++;
        if(PieceMaj[pce]) {
            pos->majPce[col]++;
        } else {
            pos->minPce[col]++;
        }
    } else {
        SETBIT(pos->pawns[col], SQ64(sq));
        SETBIT(pos->pawns[BOTH], SQ64(sq));
    }

    pos->material[col] += PieceVal[pce];
    pos->pList[pce][pos->pceNum[pce]++] = sq;
}

static void MovePiece(const int from, const int to, S_BOARD *pos) {
    
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    
    int index = 0;
    int pce = pos->pieces[from];
    int col = PieceCol[pce];

#ifdef DEBUG
    int t_pceNum = FALSE;
#endif
   
    HASH_PCE(pce, from);
    pos->pieces[from] = EMPTY;

    HASH_PCE(pce, to);
    pos->pieces[to] = pce;
    
    if(!PieceBig[pce]){
        CLRBIT(pos->pawns[col], SQ64(from));
        CLRBIT(pos->pawns[BOTH], SQ64(from));
        SETBIT(pos->pawns[col], SQ64(to));
        SETBIT(pos->pawns[BOTH], SQ64(to));
    }
   
    for(index = 0; index < pos->pceNum[pce]; index++) {
        if(pos->pList[pce][index] == from) {
            pos->pList[pce][index] = to;
#ifdef DEBUG
            int t_pceNum = TRUE;
#endif
            break;
        }
    }
    ASSERT(t_pceNum);
}

int MakeMove(S_BOARD *pos, int move) {

    ASSERT(CheckBoard(pos));

    int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(SideValid(side));
    ASSERT(PieceValid(pos->pieces[from]));

    pos->history[pos->hisPly].posKey = pos->posKey;

    if(move & MFLAGEP) {
        if(side == WHITE) {
            ClearPiece(to - 10, pos);
        } else {
            ClearPiece(to + 10, pos);
        }
    } else if(move & MFLAGCA) {
        switch(to) {
            case C1:
                MovePiece(A1, D1, pos);
            break;
            case C8:
                MovePiece(A8, D8, pos);
            break;
            case G1:
                MovePiece(H1, F1, pos);
            break;
            case G8:
                MovePiece(H8, F8, pos);
            break;
            default: ASSERT(FALSE); break;
        }
    }

    if(pos->enPas != NO_SQ) HASH_EP;
    HASH_CA; // hash out old

    pos->history[pos->hisPly].move = move;
    pos->history[pos->hisPly].fiftyMove = pos->fiftyMove;
    pos->history[pos->hisPly].enPas = pos->enPas;
    pos->history[pos->hisPly].castlePerm = pos->castlePerm;

    pos->castlePerm &= CastlePerm[from];
    pos->castlePerm &= CastlePerm[to];
    pos->enPas = NO_SQ;

    HASH_CA; // hash in new

    int captured = CAPTURED(move);
    pos->fiftyMove++;

    if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        ClearPiece(captured, pos);
        pos->fiftyMove = 0;
    }

    pos->hisPly++;
    pos->ply++;

    if(PiecePawn[pos->pieces[from]]) {
        pos->fiftyMove = 0;
        if(move & MFLAGPS) {
            if(side == WHITE) {
                pos->enPas = from + 10;
                ASSERT(RanksBrd[pos->enPas] == RANK_3);
            } else {
                pos->enPas = from - 10;
                ASSERT(RanksBrd[pos->enPas] == RANK_6);
            }
            HASH_EP;
        }
    }

    MovePiece(from ,to, pos);

    int prpce = PROMOTED(move);
    if(prpce != EMPTY) {
        ASSERT(PieceValid(prpce) && !PiecePawn[prpce]);
        ClearPiece(to, pos);
        AddPiece(to, pos, prpce); 
    }

    if(PieceKing[pos->pieces[to]]) {
        pos->KingSq[pos->side] = to;
    }

    pos->side ^= 1;
    HASH_SIDE;

    ASSERT(CheckBoard(pos));

    if(SqAttacked(pos->KingSq[side], pos->side, pos)) {
        // TakeMove(pos);
        return FALSE;
    }

    return TRUE;

}