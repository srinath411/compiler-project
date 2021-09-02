#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

int* initBitArr(int numElements);
void setBit(int* bitArr, int pos);
void clearBit(int* bitArr, int pos);
void clearAllBits(int* bitArr, int numElements);
int isBitSet(int* bitArr, int pos);
void computeBitArrUnion(int* bitArrDest, int* bitArrSource, int numElements);
void copyBitArr(int* bitArrDest, int* bitArrSource, int numElements);
int bitArrsEqual(int* bitArr1, int* bitArr2, int numElements);

#endif