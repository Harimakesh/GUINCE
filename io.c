
#include "stdio.h"
#include "defs.h"

/* Print square in algebriac notation */
char *PrSq(const int sq) {
    
    static char SqStr[3];

    int file = FilesBrd[sq];
    int rank = RanksBrd[sq];

    sprintf(SqStr, "%c%c" , ('a' + file), ('1' + rank));

    return SqStr;
}

char *PrMove(const int move) {

    static char MvStr[6];

    int ff = FilesBrd[FROMSQ(move)];
    int rf = RanksBrd[FROMSQ(move)];
    int ft = FilesBrd[TOSQ(move)];
    int rt = RanksBrd[TOSQ(move)];

    int promoted = PROMOTED(move);

    if(promoted) {
        char pchar = 'q';
        if(IsKn(promoted)){
            pchar = 'n';
        } else if(IsRQ(promoted) && !IsBQ(promoted)) {
            pchar = 'r';
        } else if(!IsRQ(promoted) && IsBQ(promoted)) {
            pchar = 'b';
        }
        sprintf(MvStr, "%c%c%c%c%c" , ('a' + ff), ('1'+ rf), ('a' + ft) , ('1' + rt), pchar);
    } else {
        sprintf(MvStr, "%c%c%c%c" , ('a' + ff), ('1'+ rf), ('a' + ft) , ('1' + rt));
    }

    return MvStr;
}

void PrintMoveList(const S_MOVELIST *list) {
    int index = 0;
    int score = 0;
    int move = 0;
    printf("MoveList : \n", list->count);

    for(index = 0; index < list->count; index++) {

        move = list->moves[index].move;
        score = list->moves[index].score;

        printf( "Move: %d > %s  %d %d (score: %d)\n", index + 1, PrMove(move), FROMSQ(move),TOSQ(move),score);
    }
    printf("MoveList Total %d moves:\n\n", list->count);
}


