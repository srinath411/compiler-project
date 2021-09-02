#include <stdlib.h>
#define INT_SIZE ((int)sizeof(int) * 8)

int* initBitArr(int numElements) {
    int arrSize = numElements / INT_SIZE + 1;
    int* bitArr = (int*) malloc(arrSize * sizeof(int));
    for(int i = 0; i < arrSize; i++) {
        bitArr[i] = 0;
    }
    return bitArr;
}

void setBit(int* bitArr, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    bitArr[arrPos] |= 1 << bitPos;
}

void clearBit(int* bitArr, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    bitArr[arrPos] &= ~(1 << bitPos);   
}

void clearAllBits(int* bitArr, int numElements) {
    int arrSize = numElements / INT_SIZE + 1;
    for(int i = 0; i < arrSize; i++) {
        bitArr[i] = 0;
    }
}

int isBitSet(int* bitArr, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    return bitArr[arrPos] & (1 << bitPos);
}

void computeBitArrUnion(int* bitArrDest, int* bitArrSource, int numElements) {
    int arrSize = numElements / INT_SIZE + 1;
    for(int i = 0; i < arrSize; i++) {
        bitArrDest[i] |= bitArrSource[i];
    }
}

void copyBitArr(int* bitArrDest, int* bitArrSource, int numElements) {
    int arrSize = numElements / INT_SIZE + 1;
    for(int i = 0; i < arrSize; i++) {
        bitArrDest[i] = bitArrSource[i];
    }
}

int bitArrsEqual(int* bitArr1, int* bitArr2, int numElements) {
    int arrSize = numElements / INT_SIZE + 1;
    for(int i = 0; i < arrSize; i++) {
        if (bitArr1[i] != bitArr2[i]) {
            return 0;
        }
    }
    return 1;
} 