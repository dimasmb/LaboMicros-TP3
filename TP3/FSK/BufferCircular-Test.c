// Online C compiler to run C program online
#include <stdio.h>
#include "stdint.h"

#define BUFFSIZE 5
#define DATATYPE int

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

circBuffer_t mybuffer;
int16_t Buf[BUFFSIZE];

void printBuff(void);
int main() {
    mybuffer.Buffer=Buf;
    mybuffer.buffSize=BUFFSIZE;
    mybuffer.d=BUFFSIZE-1;
    mybuffer.inPtr=0;
    mybuffer.outPtr=0;
    clearBuff(&mybuffer);
    //printf("Is Buffer Empty? %d \n",isBuffEmpty(&mybuffer));
    int j= 0;
    for(j=0;j<10;j++){
        printBuff();
        printf("Is Buffer Empty? %d \n",isBuffEmpty(&mybuffer));
        insertData(&mybuffer,j);
        //getData(&mybuffer);
    }
    
    for(j=0;j<10;j++){
        getData(&mybuffer);
        printBuff();
        printf("Is Buffer Empty? %d \n",isBuffEmpty(&mybuffer));
    }
    
    
    
    

    return 0;
}

void printBuff(void){
    int i=0;
    printf("---------------------------------\n");
    for(i=0;i<BUFFSIZE;i++){
        printf("%d ",mybuffer.Buffer[i]);
    }
    printf("\n");
     printf("inPtr= %d | outPtr= %d | d= %d \n",mybuffer.inPtr,mybuffer.outPtr,mybuffer.d);
     printf("\n");
}

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