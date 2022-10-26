#include "BufferCirc.h"


int dist=BUFFSIZE-1;
int outPointer=0;
int inPointer=0;
char cirBuff[BUFFSIZE];

void clearBuff(void){

    int i=0;
    for (i=0;i<BUFFSIZE;i++){
        cirBuff[i]=0;
    }
}

int insertChar(char data){
    if(dist>0){
        cirBuff[inPointer]=data;
        dist--;
        if(inPointer>=(BUFFSIZE-1)){
            inPointer=0;
        }else{
            inPointer++;
        }
    return 1;
    }
    return 0;
   
}

char getChar(){
    char result=0;
    if(outPointer-inPointer){
        dist++;
        result=cirBuff[outPointer];
        if(outPointer>=(BUFFSIZE-1)){
            outPointer=0;
        }else{
            outPointer++;
        }
    }
    return result;
    
}

