#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char * normalizarByte(unsigned char byte);
unsigned char * moverBytes(unsigned char *bp);
unsigned char hamming(unsigned char byte);

int main()
{
    FILE *fp;
    fp = fopen("fichero.txt","r");
    fseek(fp, 0L, SEEK_END);
    long bytes = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    unsigned char * buffer =(unsigned char *)malloc(bytes*sizeof(unsigned char));
    fread (buffer,1,bytes,fp);
    fclose(fp);
    int modulo;
    if(bytes%8 == 0)
        modulo = 0;
    else
        modulo = 8-bytes%8;
    unsigned char * bytesProtegidos = (unsigned char *)calloc(2*bytes+modulo,sizeof(unsigned char));
    unsigned char * bn;
    for(int i=0;i<bytes;i++){
        bn = normalizarByte(buffer[i]);
        bytesProtegidos[2*i] = *(bn);
        bytesProtegidos[2*i+1] = *(bn+1);
    }
    free(buffer);
    int doubleBytes = 2*bytes;
    for(int i=0;i<doubleBytes;i++)
        bytesProtegidos[i] = hamming(bytesProtegidos[i]);
    int j = 0;
    for(int i=0;i<doubleBytes;i+=8){
        unsigned char * septetoBytes = moverBytes(&bytesProtegidos[i]);
        bytesProtegidos[j] = *(septetoBytes);
        bytesProtegidos[j+1] = *(septetoBytes+1);
        bytesProtegidos[j+2] = *(septetoBytes+2);
        bytesProtegidos[j+3] = *(septetoBytes+3);
        bytesProtegidos[j+4] = *(septetoBytes+4);
        bytesProtegidos[j+5] = *(septetoBytes+5);
        bytesProtegidos[j+6] = *(septetoBytes);
        j+=7;
    }
    fp =fopen("ficherosalida.txt","w");
    fwrite(bytesProtegidos,1,(int)ceil(bytes*1.75),fp);
    free(bytesProtegidos);
    fclose(fp);
    return 0;
}

unsigned char * normalizarByte(unsigned char byte){
    static unsigned char bytesNormalizados [2];
    bytesNormalizados[0] = (byte & 240) >> 4;
    bytesNormalizados[1] = byte & 15;
    return bytesNormalizados;
}

unsigned char * moverBytes(unsigned char *bp){
    static unsigned char ba[7];
    ba[0] = bp[0];
    ba[0] = ba[0] | ((*(bp+1) & 128) >> 7);
    ba[1] = bp[1] << 1;
    ba[1] = ba[1] | ((*(bp+2) & 192) >> 6);
    ba[2] = bp[2] << 2;
    ba[2] = ba[2] | ((*(bp+3) & 224) >> 5);
    ba[3] = bp[3] << 3;
    ba[3] = ba[3] | ((*(bp+4) & 240) >> 4);
    ba[4] = bp[4] << 4;
    ba[4] = ba[4] | ((*(bp+5) & 248) >> 3);
    ba[5] = bp[5] << 5;
    ba[5] = ba[5] | ((*(bp+6) & 252) >> 2);
    ba[6] = bp[6] << 6;
    ba[6] = ba[6] | (*(bp+7) >> 1);
    return ba;
}

unsigned char hamming(unsigned char byte){
    unsigned char i1 = byte & 1;
    unsigned char i2 = (byte & 2) >> 1;
    unsigned char i3 = (byte & 4) >> 2;
    unsigned char i4 = (byte & 8) >> 3;
    unsigned char c1 = (i1 ^ i2 ^ i4) << 1;
    unsigned char c2 = (i1 ^ i3 ^ i4) << 2;
    unsigned char c3 = (i2 ^ i3 ^ i4) << 4;
    i1 = i1 << 3;
    i2 = i2 << 5;
    i3 = i3 << 6;
    i4 = i4 << 7;
    return i1|i2|i3|i4|c1|c2|c3;
}


