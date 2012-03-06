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

typedef struct RingBuffer {
    void *buffer;
    volatile unsigned int writeIndex;
    volatile unsigned int readIndex;
    size_t capacity;
    size_t count;
    size_t eleSize;
} RingBuffer;

// Nelem will be rounded up to a power of 2
RingBuffer * rbInit(size_t nelem, size_t elemsize);
void rbDestroy(RingBuffer *rb);

void rbPush(RingBuffer *rb, void *elem);
void rbPushN(RingBuffer *rb, void *elem, int n);

void rbPop(RingBuffer *rb, void *elem);
void rbPopN(RingBuffer *rb, void *elem, int n);

//If you want to increment the read index a number other than n
void rbPopOption(RingBuffer *rb, void *elem, int n, int readIndexIncrement);

size_t rbWriteCapacity(RingBuffer *rb);
size_t rbReadCapacity(RingBuffer *rb);

int rbFull(RingBuffer *rb);
int rbEmpty(RingBuffer *rb);

int rbCanPush(RingBuffer *rb, int n);
int rbCanPop(RingBuffer *rb, int n);

void rbPrintInt(RingBuffer *rb);
void rbPrintFloat(RingBuffer *rb);

int roundNextPowerof2(int inVal);
