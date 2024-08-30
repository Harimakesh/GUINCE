// perft.c 

#include "defs.h"
#include "stdio.h"

void perft(int depth, S_BOARD *pos, long *leafnodes) {
    ASSERT(CheckBoard(pos));

    if(depth == 0) {
        (*leafnodes)++;
        return;
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);
    // PrintMoveList(list);
    int moveNum = 0;
    int move;
    for(moveNum = 0; moveNum < list->count; ++moveNum) {
        move = list->moves[moveNum].move;
        if( !MakeMove(pos,  move)) {
            continue;
        }
        perft(depth - 1, pos, leafnodes);
        TakeMove(pos);
    }
    return;
}

void perfTest(int depth, S_BOARD *pos, long *leafnodes) {
    ASSERT(CheckBoard(pos));

    // PrintBoard(pos);
    // printf("\nStarting Test to DEPTh : %d\n", depth);

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);
    int move;
    int movenum = 0;
    for(movenum = 0; movenum < list->count; ++movenum) {
        move = list->moves[movenum].move;
        if(!MakeMove(pos, move)){
            continue;
        }
        // long cumnodes = *leafnodes;
        perft(depth - 1, pos, leafnodes);
        TakeMove(pos);
        // long oldnodes = *leafnodes - cumnodes;
        // printf("move %d : %s : %ld\n", movenum + 1, PrMove(move), oldnodes);
    }

    // printf("test complete : %ld nodes visited\n", leafnodes);
}

