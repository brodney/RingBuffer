/*
 
Copyright (c) 2012 R. Brett Jones

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RingBuffer.h"


int rbRoundNextPowerof2(int inVal) {
    int pow2 = 1;
    while (inVal > pow2 && pow2 < (1 << 30) - 1) {
        pow2 = pow2 << 1;
    }
    return pow2;  
}

RingBuffer * rbInit(size_t nelem, size_t elemsize) {
    RingBuffer *rb = malloc(sizeof(RingBuffer));
    rb->capacity = rbRoundNextPowerof2(nelem);
    rb->eleSize = elemsize;
    rb->readIndex = rb->writeIndex = 0;
    rb->buffer = calloc(rb->capacity, elemsize);
    
    return rb;
}

void rbDestroy(RingBuffer *rb) {
    free(rb->buffer);
    free(rb);
}

void rbPush(RingBuffer *rb, void *elem) {
    
    memmove(rb->buffer + (rb->writeIndex++ & (rb->capacity - 1))*rb->eleSize,elem , rb->eleSize);
    
}
void rbPushN(RingBuffer *rb, void *elem, int n) { 
    int index = 0;
    while (index < n) {
        rbPush(rb, elem + (index++ * rb->eleSize));
    }
}

void rbPop(RingBuffer *rb, void *elem) { 
    memmove(elem, rb->buffer + (rb->readIndex++ & (rb->capacity - 1))*rb->eleSize, rb->eleSize);
}

void rbPopN(RingBuffer *rb, void *elem, int n) {
    int index = 0;
    while (index < n) {
        rbPop(rb, elem + (index++ * rb->eleSize));
    }
}

//If you want to increment the readIndex by a value other than N
void rbPopOption(RingBuffer *rb, void *elem, int n, int readIndexIncrement) {
    rbPopN(rb, elem, n);
    rb->readIndex -= n - readIndexIncrement;  
}

size_t rbWriteCapacity(RingBuffer *rb) {
    return rb->capacity - rbReadCapacity(rb);
}

size_t rbReadCapacity(RingBuffer *rb) {
    return rb->writeIndex - rb->readIndex;
}

int rbFull(RingBuffer *rb) {
    return rbWriteCapacity(rb) == 0 ? 1 : 0;
}
int rbEmpty(RingBuffer *rb) {
    return rbReadCapacity(rb) == 0 ? 1 : 0;
}

int rbCanPush(RingBuffer *rb, int n) { 
    return rbWriteCapacity(rb) >= n ? 1 : 0;
}

int rbCanPop(RingBuffer *rb, int n) {
    return rbReadCapacity(rb) >= 0 ? 1 : 0;
}

void rbPrintDetails(RingBuffer *rb) {
    printf("ri:%i wi:%i c:%i f:%i e:%i wc:%i rc:%i\n",(int)rb->readIndex,(int)rb->writeIndex,(int)rb->capacity,rbFull(rb),rbEmpty(rb),(int)rbWriteCapacity(rb),(int)rbReadCapacity(rb));
}

void rbPrintInt(RingBuffer *rb) {
    int i;
    void *bufferStart = rb->buffer;
    for (i = 0; i < rb->capacity; i++) {
        printf("%i,",*((int *)bufferStart + i));
    }
    printf("\n");
}

void rbPrintFloat(RingBuffer *rb) {
    int i;
    void *bufferStart = rb->buffer;
    for (i = 0; i < rb->capacity; i++) {
        printf("%f,",*((float *)bufferStart+ i));
    }
    printf("\n");
}

/*
int main(void) {
    printf("\n::Testing each function::\n");
    RingBuffer *rb = rbInit(14,sizeof(int));
    
    printf("\n::Push one test::\n");
    int five = 5;
    int popped = 0;
    rbPush(rb,&five);
    rbPrintDetails(rb);
    rbPrintInt(rb);
    
    printf("\n::Push array test::\n");
    int nTest = 20;
    int array[nTest];
    int i = 0;
    for (i = 0; i < nTest; i++) {
        array[i] = i;
        printf("%i,",array[i]);
    }
    printf("\n");
    rbPushN(rb,array,nTest);
    rbPrintDetails(rb);
    rbPrintInt(rb);
    
    printf("\n::Pop one test::\n");
    int first, second;
    rbPop(rb, &first);
    rbPop(rb, &second);
    rbPrintDetails(rb);
    printf("1:%i 2:%i\n",first, second);
    
    printf("\n::Pop array test::\n");
    nTest = 10;
    int *popArray = calloc(nTest, sizeof(int));
    rbPopN(rb, popArray, nTest);
    printf("%i from popN: ",nTest);
    for (i = 0; i < nTest; i++) {
        printf("%i,",popArray[i]);
    }
    printf("\n");
    rbPrintDetails(rb);
    rbPrintInt(rb);
    printf("\n::Pop array optional increment test::\n");
    memset(popArray,0,nTest * sizeof(int));
    rbPopOption(rb,popArray,nTest,4);
    for (i = 0; i < nTest; i++) {
        printf("%i,",popArray[i]);
    }
    printf("\n");
    rbPrintDetails(rb);

  return 0;
}*/
