
#include "../includes/r3c-string.hpp"

#include <string.h>


// *** CONSTANTS *** //

#define ALLOC_STACK_SIZE 64


// *** CONSTRUCTION *** //

// Initializes this string block stack.
void R3CStringBlockStack::init() {
    this->stackStartPtr = new char* [ALLOC_STACK_SIZE];
    memset(this->stackStartPtr, 0, ALLOC_STACK_SIZE * sizeof(char*));
    this->stackStartPtr[0] = this->nextStrPtr;
}

R3CStringBlockStack::R3CStringBlockStack() :
    R3CStringBlock(),
    stackLevel(0),
    stackStartPtr(NULL),
    stackStartPtrAlloc(ALLOC_STACK_SIZE)
{
    this->init();
}

R3CStringBlockStack::R3CStringBlockStack(int kbPerBlock) :
    R3CStringBlock(kbPerBlock),
    stackLevel(0),
    stackStartPtr(NULL),
    stackStartPtrAlloc(ALLOC_STACK_SIZE)
{
    this->init();
}


// *** DESTRUCTION *** //

R3CStringBlockStack::~R3CStringBlockStack() {
    if ( this->stackStartPtr != NULL ) {
        delete[] this->stackStartPtr;
    }
}


// *** MANAGE STRING STACK *** //

// Retrieves the current stack level.
int R3CStringBlockStack::getStackLevel() {
    return( this->stackLevel );
}

// Pushes the stack to a new level.
int R3CStringBlockStack::push() {
    char** oldStackStartPtr;
    int oldStackStartPtrAlloc;
    size_t stackStartPtrSize;

    // Increase the stack level
    this->stackLevel++;

    // Check if this stack level exceeds the allocated stack start pointer
    // storage
    if ( this->stackLevel >= this->stackStartPtrAlloc ) {
        // Keep track of the old stack start pointers
        oldStackStartPtr = this->stackStartPtr;
        oldStackStartPtrAlloc = this->stackStartPtrAlloc;

        // Allocate new stack start pointer space
        this->stackStartPtrAlloc <<= 1;
        this->stackStartPtr = new char* [this->stackStartPtrAlloc];

        // Initialize all new pointers to NULL
        stackStartPtrSize = this->stackStartPtrAlloc * sizeof(char*);
        memset(this->stackStartPtr, 0, stackStartPtrSize);

        // Copy the old pointers to the new pointer space
        stackStartPtrSize = oldStackStartPtrAlloc * sizeof(char*);
        memcpy(this->stackStartPtr, oldStackStartPtr, stackStartPtrSize);

        // Destroy the old stack start pointers
        delete[] oldStackStartPtr;
    }

    // Set the stack start pointer to the current position in this storage
    // block
    this->stackStartPtr[this->stackLevel] = this->nextStrPtr;

    // Return the new stack level
    return( this->stackLevel );
}

// Pops off the current stack level.
int R3CStringBlockStack::pop() {
    char* ptrToFind;
    int foundInMemBlock;
    char* foundInMemBlockPtr;
    bool isFound;
    int bytesToClear;

#ifndef R3C_NOERRCHECK
    // Check if the stack is empty, and thus cannot be popped
    if ( this->stackLevel <= 0 ) {
        throw R3CERR_STR_EMPTYSTACK;
    }
#endif

    // Find the storage block that this stack level began allocating strings
    ptrToFind = this->stackStartPtr[this->stackLevel];
    foundInMemBlock = this->currMemBlock;
    isFound = false;
    while ( !isFound ) {
        foundInMemBlockPtr = this->memBlock[foundInMemBlock];
        if (
            (ptrToFind >= foundInMemBlockPtr) &&
            (ptrToFind < foundInMemBlockPtr + this->bytesPerBlock)
        ) {
            // This stack level began allocation in storage block
            // foundInMemBlock
            isFound = true;
        } else {
            // This stack level began allocation in an earlier storage block
            // Clear this storage block and move to the previous
            memset(foundInMemBlockPtr, 0, this->bytesPerBlock);
            foundInMemBlock--;
        }
    }

    // Clear all strings beyond this point in storage block foundInMemBlock
    bytesToClear =
        this->bytesPerBlock - (int)(ptrToFind - foundInMemBlockPtr);
    memset(ptrToFind, 0, bytesToClear);

    // Update the current storage block
    this->currMemBlock = foundInMemBlock;
    this->nextStrPtr = ptrToFind;
    this->bytesUsedInBlock = this->bytesPerBlock - bytesToClear;
    
    // Update the stack level
    this->stackStartPtr[this->stackLevel] = NULL;
    this->stackLevel--;

    // Return the new stack level
    return( this->stackLevel );
}
