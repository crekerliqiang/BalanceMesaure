#include "rc4.h"
#include "string.h"
static int iS[256] = {0};   
static int iK[256] = {0}; 
void HloveyRC4(int *aInput,int *aKey,int input_len,int *bOutput)   
{
    int i=0,j=0,temp=0,x=0,t=0,iY=0,iCY=0;
    for (i=0;i<256;i++) {
    	iS[i]=i;   
    }
    j = 1;
    for (i= 0;i<256;i++)   
    {
        iK[i]= aKey[i % 24];
    }
    j=0;   
    for (i=0;i<255;i++)   
    {   
        j=(j+iS[i]+iK[i]) % 256;   
        temp = iS[i];   
        iS[i]=iS[j];   
        iS[j]=temp;   
    }   
    i=0;   
    j=0;   
    for(x = 0;x<input_len;x++)   
    {
        i = (i+1) % 256;   
        j = (j+iS[i]) % 256;   
        temp = iS[i];   
        iS[i]=iS[j];   
        iS[j]=temp;   
        t = (iS[i]+(iS[j] % 256)) % 256;   
        iY = iS[t];   
        iCY = iY;   
        bOutput[x] =( aInput[x] ^ iCY) ;      
    }
		memset(iS,0,256);memset(iK,0,256);
}  
