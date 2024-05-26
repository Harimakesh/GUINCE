
#include "stdio.h"
#include "defs.h"
#define MOVE(f, t , ca, pro, fl) ( (f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[sq] == OFFBOARD) 

void AddQuietMove ( const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}
void AddCaptureMove ( const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}
void AddEnPassantMove ( const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

void AddWhitePawnCapMove( const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list) {
    if(RanksBrd[from] == RANK_7) {
        AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), list);
        AddCaptureMove(pos, MOVE(from, to, cap, wR, 0), list);
        AddCaptureMove(pos, MOVE(from, to, cap, wB, 0), list);
        AddCaptureMove(pos, MOVE(from, to, cap, wN, 0), list);
    }
    else {
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

void AddWhitePawnMove( const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list) {
    if(RanksBrd[from] == RANK_7) {
        AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
        AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
        AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
        AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
    }
    else {
        AddMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
    }
}
void GenerateAllMoves( const S_BOARD *pos, S_MOVELIST *list) {
    
}