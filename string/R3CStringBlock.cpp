
#include "../includes/r3c.hpp"
#include "../includes/r3c-string.hpp"

#include <string.h>


// *** CONSTANTS *** //

#define MAX_KB_PER_BLOCK 64
#define ALLOC_BLOCK_PTR_SIZE 16


// *** CONSTRUCTION *** //

// Initializes this string storage block.
void R3CStringBlock::init(int kbPerBlock) {
    if ( kbPerBlock > MAX_KB_PER_BLOCK ) kbPerBlock = MAX_KB_PER_BLOCK;
    this->memBlock = new char* [ALLOC_BLOCK_PTR_SIZE];
    memset(this->memBlock, 0, ALLOC_BLOCK_PTR_SIZE * sizeof(char*));
    this->memBlock[0] = new char [this->bytesPerBlock];
    memset(this->memBlock[0], 0, this->bytesPerBlock);
    this->nextStrPtr = this->memBlock[0];
}

// Creates a new string storage block.
R3CStringBlock::R3CStringBlock() :
    bytesPerBlock(4096),
    memBlockAlloc(ALLOC_BLOCK_PTR_SIZE),
    memBlock(NULL),
    currMemBlock(0),
    nextStrPtr(NULL),
    bytesUsedInBlock(0)
{
    init(4);
}

// Creates a new string storage block.
R3CStringBlock::R3CStringBlock(int kbPerBlock) :
    bytesPerBlock(kbPerBlock << 10),
    memBlockAlloc(ALLOC_BLOCK_PTR_SIZE),
    memBlock(NULL),
    currMemBlock(0),
    nextStrPtr(NULL),
    bytesUsedInBlock(0)
{
#ifndef R3C_NOERRCHECK
    if ( kbPerBlock < 1 ) throw R3CERR_ILLEGALARGUMENT;
#endif
    this->init(kbPerBlock);
}


// *** DESTRUCTION *** //

// Destructor.
R3CStringBlock::~R3CStringBlock() {
    int loop;
    if ( this->memBlock != NULL ) {
        for ( loop = 0; loop < this->memBlockAlloc; loop++ ) {
            if ( this->memBlock[loop] != NULL ) {
                delete[] this->memBlock[loop];
            }
        }
        delete[] this->memBlock;
    }
}


// *** MANAGE STRINGS *** //

// Ensures the current storage block has enough space for the given number of
// characters.
void R3CStringBlock::ensureBlockCapacity(int charsToAlloc) {
    char** oldMemBlock;
    int oldMemBlockAlloc;
    size_t memBlockPtrSize;

    // Check if this allocation will exceed the size of the current block
    if ( (this->bytesUsedInBlock + charsToAlloc) >= this->bytesPerBlock ) {
        // Move to the next available storage block
        this->currMemBlock++;

        // Check if we need to allocate more storage block pointer space
        if ( this->currMemBlock >= this->memBlockAlloc ) {
            // Keep track of the old block pointers
            oldMemBlock = this->memBlock;
            oldMemBlockAlloc = this->memBlockAlloc;

            // Allocate new storage block pointer space
            this->memBlockAlloc <<= 1;
            this->memBlock = new char* [this->memBlockAlloc];

            // Initialize all new pointers to NULL
            memBlockPtrSize = this->memBlockAlloc * sizeof(char*);
            memset(this->memBlock, 0, memBlockPtrSize);

            // Copy the old pointers into the new pointer space
            memBlockPtrSize = oldMemBlockAlloc * sizeof(char*);
            memcpy(this->memBlock, oldMemBlock, memBlockPtrSize);

            // Destroy the old block pointers
            delete[] oldMemBlock;
        }

        // If this storage block has not been allocated, allocate it
        if ( this->memBlock[this->currMemBlock] == NULL ) {
            this->memBlock[this->currMemBlock] =
                new char [this->bytesPerBlock];
        }

        // Initialize the new storage block
        memset(this->memBlock[this->currMemBlock], 0, this->bytesPerBlock);
        this->nextStrPtr = this->memBlock[this->currMemBlock];
        this->bytesUsedInBlock = 0;
    }
}

// Determines if a string needing the given storage space should be allocated
// in its own block.
bool R3CStringBlock::shouldAllocAlone(int charsToAlloc) {
    return( charsToAlloc > (this->bytesPerBlock >> 1) );
}

// Inserts the given character string into this string block.
char* R3CStringBlock::insertString(const char* str, int charsToAlloc) {
    char* result;
    if ( this->shouldAllocAlone(charsToAlloc) ) {
        result = new char [charsToAlloc];
        strcpy(result, str);
    } else {
        this->ensureBlockCapacity(charsToAlloc);
        strcpy(this->nextStrPtr, str);
        result = this->nextStrPtr;
        this->nextStrPtr += charsToAlloc;
        this->bytesUsedInBlock += charsToAlloc;
    }
    return( result );
}

// Adds the given character string to this string block.
char* R3CStringBlock::addString(const char* str) {
    if ( str == NULL ) return( NULL );
    return( this->insertString(str, (int)strlen(str) + 1) );
}

// Adds the given character string to this string block, providing enough
// space for the string to grow to some known maximum length.
char* R3CStringBlock::addString(const char* str, int maxLength) {
    int charsToAlloc;
    if ( str == NULL ) return( NULL );
    charsToAlloc = (int)strlen(str);
    if ( maxLength > charsToAlloc ) charsToAlloc = maxLength;
    charsToAlloc++;
    return( this->insertString(str, charsToAlloc) );
}

// Adds the given string to this string block.
char* R3CStringBlock::addString(R3CString* str) {
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
    return( this->insertString(str->getChars(), str->getLength() + 1) );
}

// Adds the given string to this string block, providing enough space for the
// string to grow to some known maximum length.
char* R3CStringBlock::addString(R3CString* str, int maxLength) {
    int charsToAlloc;
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
    charsToAlloc = str->getLength();
    if ( maxLength > charsToAlloc ) charsToAlloc = maxLength;
    charsToAlloc++;
    return( this->insertString(str->getChars(), charsToAlloc) );
}
