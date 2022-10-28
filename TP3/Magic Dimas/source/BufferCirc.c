#include "BufferCirc.h"



int isBuffEmpty(circBuffer_t *B){
    int result=1;
    if((B->outPtr)-(B->inPtr))
        result=0;
    return result;

}


void clearBuff(circBuffer_t *B){

    int i=0;
    for (i=0;i<(B->buffSize);i++){
        B->Buffer[i]=0;
    }
}

int insertData(circBuffer_t *B,DATATYPE data){
    if((B->d)>0){
        B->Buffer[B->inPtr]=data;
        (B->d)--;
        if((B->inPtr)>=((B->buffSize)-1)){
            B->inPtr=0;
        }else{
            (B->inPtr)++;
        }
    return 1;
    }
    return 0;
}

DATATYPE getData(circBuffer_t *B){
	DATATYPE result=0;
    if((B->outPtr)-(B->inPtr)){
        (B->d)++;
        result=B->Buffer[B->outPtr];
        if((B->outPtr)>=((B->buffSize)-1)){
            B->outPtr=0;
        }else{
            (B->outPtr)++;
        }
    }
    return result;
    
}

