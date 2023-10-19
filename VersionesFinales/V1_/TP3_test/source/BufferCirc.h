#include <stdio.h>
#include "stdint.h"

#ifndef _BUFFERCIRC_H_
#define _BUFFERCIRC_H_

//Define buffer datatype.
//#define DATATYPE int16_t

typedef struct{
    int d; //tiene que ser buffsieze-1;
    int outPtr;
    int inPtr;
    int16_t *Buffer;
    int buffSize;
}circBuffer_t;

int insertData(circBuffer_t *B,int16_t data);
int16_t getData(circBuffer_t *B);
void clearBuff(circBuffer_t *B);
int isBuffEmpty(circBuffer_t *B);
#endif // _TEMPLATE_H_
