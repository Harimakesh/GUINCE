#include "defs.h"
#include "stdio.h"
// the directions in which a piece can move (compass)
const int KnDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 }; 
const int RkDir[4] = { -1, -10,	1, 10 };
const int BiDir[4] = { -9, -11, 11, 9 };
const int KiDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };


int SqAttacked(const int sq, const int side, const S_BOARD *pos) {

    int pce, index, t_sq, dir;
    
    ASSERT(SqOnBoard(sq));
    ASSERT(SideValid(side));
    ASSERT(CheckBoard(pos));

    if(side == WHITE) {
        if(pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP){
            return TRUE;
        }
    } else {
        if(pos->pieces[sq + 11] == bP || pos->pieces[sq + 9] == bP){
            return TRUE;
        }
    }
    
    // knights
    for(index = 0; index < 8; ++index) {
        pce = pos->pieces[sq + KnDir[index]];
        if(pce != OFFBOARD && IsKn(pce) && PieceCol[pce] == side){
            // printf("%c %d %d %c ",SideChar[PieceCol[pce]], pce ,sq + KnDir[index], SideChar[side]);
            return TRUE;
        }
    }
    

    //rooks and queens
    for(index = 0; index < 4; ++index) {
        // go along 'dir' until pce encountered
        dir = RkDir[index];
        t_sq = sq + dir;

        pce = pos->pieces[t_sq];
        while (pce != OFFBOARD) {
            if( pce != EMPTY) {
                if(IsRQ(pce) && PieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
        
    }
    
    // bishop and queen
    for(index = 0; index < 4; ++index) {
        // go along 'dir' until pce encountered or OFFBOARD
        dir = BiDir[index];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while (pce != OFFBOARD) {
            if( pce != EMPTY) {
                if(IsBQ(pce) && PieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
        
    }
    

    // kings
    for(index = 0; index < 8; ++index) {
        pce = pos->pieces[sq + KiDir[index]];
        if(pce != OFFBOARD && IsKi(pce) && PieceCol[pce] == side){
            return TRUE;
        }
    }
   

    return FALSE;

}