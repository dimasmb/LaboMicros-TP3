#include <stdio.h>
#include "stdint.h"

#ifndef _BUFFERCIRC_H_
#define _BUFFERCIRC_H_

//Define buffer datatype.
#define DATATYPE int16_t
#define BUFFSIZE 100

/*  CREATE BUFFER:
        circBuffer_t mybuffer;
        DATATYPE Buf[BUFFSIZE];

    INIT BUFFER:
        mybuffer.Buffer=Buf;
        mybuffer.buffSize=BUFFSIZE;
        mybuffer.d=BUFFSIZE-1;
        mybuffer.inPtr=0;
        mybuffer.outPtr=0;
        clearBuff(&mybuffer);


*/

typedef struct{
    int d; //tiene que ser buffsieze-1;
    int outPtr;
    int inPtr;
    DATATYPE *Buffer;
    int buffSize;
}circBuffer_t;

int insertData(circBuffer_t *B,DATATYPE data);
DATATYPE getData(circBuffer_t *B);
void clearBuff(circBuffer_t *B);
int isBuffEmpty(circBuffer_t *B);
#endif // _TEMPLATE_H_
