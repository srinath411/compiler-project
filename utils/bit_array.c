#include <stdlib.h>
#define INT_SIZE ((int)sizeof(int) * 8)

/*
 * Initializes and returns bit array
 */
int* initBitArr(int numElements) {
    // Number of bits required = numElements. Round up if necessary
    int arrSize = numElements / INT_SIZE + (numElements % INT_SIZE != 0);
    int* bitArr = (int*) malloc(arrSize * sizeof(int));
    for(int i = 0; i < arrSize; i++) {
        bitArr[i] = 0;
    }
    return bitArr;
}

/*
 * Sets bit at given position
 */
void setBit(int* bitArr, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    bitArr[arrPos] |= 1 << bitPos;
}

/*
 * Clears bit at given position
 */
void clearBit(int* bitArr, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    bitArr[arrPos] &= ~(1 << bitPos);   
}

/*
 * Clears all bits in bit array
 */
void clearAllBits(int* bitArr, int numElements) {
    int arrSize = numElements / INT_SIZE + (numElements % INT_SIZE != 0);
    for(int i = 0; i < arrSize; i++) {
        bitArr[i] = 0;
    }
}

/*
 * Checks if bit is set at given position
 */
int isBitSet(int* bitArr, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    return bitArr[arrPos] & (1 << bitPos);
}

/*
 * Computes bitwise union of two bit arrays
 */
void computeBitArrUnion(int* bitArrDest, int* bitArrSource, int numElements) {
    int arrSize = numElements / INT_SIZE + (numElements % INT_SIZE != 0);
    for(int i = 0; i < arrSize; i++) {
        bitArrDest[i] |= bitArrSource[i];
    }
}

/*
 * Copies contents of source bit array into destination bit array
 */
void copyBitArr(int* bitArrDest, int* bitArrSource, int numElements) {
    int arrSize = numElements / INT_SIZE + (numElements % INT_SIZE != 0);
    for(int i = 0; i < arrSize; i++) {
        bitArrDest[i] = bitArrSource[i];
    }
}

/*
 * Checks if two bit arrays are equal
 */
int bitArrsEqual(int* bitArr1, int* bitArr2, int numElements) {
    int arrSize = numElements / INT_SIZE + (numElements % INT_SIZE != 0);
    for(int i = 0; i < arrSize; i++) {
        if (bitArr1[i] != bitArr2[i]) {
            return 0;
        }
    }
    return 1;
} 