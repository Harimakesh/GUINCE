// perfttest.c

#include "defs.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


void ParseFEN() {

}
int flag = 1;

void ParseLine(char *line){
    char fen[256];
    long leafnodes;
    S_BOARD pos[1];
    int i = 0;
    while (*line != ';') {
        fen[i++] = *line;
        line++; 
    }
    fen[i] = '\0';
    ParseFen(fen, pos);
    printf("\nFEN : %s\n", fen);
    while(*line){
        if(*line == 'D'){
            line++;
            int depth = atoi(line);
            if(depth == 6)
                break;
            line += 2;
            long leafnodes = 0;
            perfTest(depth, pos, &leafnodes);
            if(leafnodes == atol(line)) {
                printf(" depth : %d nodes : %ld \n",  depth, leafnodes);
            } else {
                printf("ERROR on depth : %d \n", depth);
                flag = 0;
            }
        }
        line++;
    }
    return;
}

void ParseFile(){

    // parse each line
    // store results until depth 
    FILE *perftfile;
    char line[256];

    perftfile = fopen("perftsuite.epd", "r");

    if(perftfile == NULL){
        printf("File Not Found");
        return;
    } else {
        while (fgets(line, 256, perftfile)) {
            ParseLine(line);
            memset(line, 0, sizeof(line));
        }
    }

    if(flag){
        printf("\n perft test success");
    }

}
