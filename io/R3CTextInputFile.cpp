
#include "../includes/r3c.hpp"
#include "../includes/r3c-string.hpp"
#include "../includes/r3c-io.hpp"

#include <stdio.h>


// *** CONSTANTS *** //

const char* READ_MODE = "r";


// *** CONSTRUCTION *** //

R3CTextInputFile::R3CTextInputFile() :
    fileHandle(NULL)
{
}

R3CTextInputFile::R3CTextInputFile(const char *inputFilename) :
    fileHandle(NULL)
{
    this->open(inputFilename);
}

R3CTextInputFile::R3CTextInputFile(R3CString *inputFilename) :
    fileHandle(NULL)
{
    this->open(inputFilename);
}


// *** DESTRUCTION *** //

R3CTextInputFile::~R3CTextInputFile() {
    if ( this->fileHandle != NULL ) {
        fclose(this->fileHandle);
    }
}


// *** OPEN STREAM *** //

void R3CTextInputFile::open(const char *inputFilename) {
#ifndef R3C_NOERRCHECK
    if ( inputFilename == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( this->fileHandle != NULL ) throw R3CERR_IO_STREAMALREADYOPEN;
#endif
    this->fileHandle = fopen(inputFilename, READ_MODE);
    if ( this->fileHandle == NULL ) throw R3CERR_IO_STREAMNOTFOUND;
}

void R3CTextInputFile::open(R3CString *inputFilename) {
#ifndef R3C_NOERRCHECK
    if ( inputFilename == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( this->fileHandle != NULL ) throw R3CERR_IO_STREAMALREADYOPEN;
#endif
    this->fileHandle = fopen(inputFilename->getChars(), READ_MODE);
    if ( this->fileHandle == NULL ) throw R3CERR_IO_STREAMNOTFOUND;
}


// *** CHECK FOR STREAM READINESS *** //

bool R3CTextInputFile::ready() {
#ifndef R3C_NOERRCHECK
    if ( this->fileHandle == NULL ) throw R3CERR_IO_STREAMNOTOPEN;
#endif
    return( true );
}


// *** READ CHARACTERS FROM THE FILE *** //

int R3CTextInputFile::readChar() {
    int result;
#ifndef R3C_NOERRCHECK
    if ( this->fileHandle == NULL ) throw R3CERR_IO_STREAMNOTOPEN;
#endif
    result = getc(this->fileHandle);
    if ( ferror(this->fileHandle) != 0 ) throw R3CERR_IO_EXCEPTION;
    return( result );
}

int R3CTextInputFile::readChars(R3CString *targetStr, int numChars) {
    int charsRead;
    int curChar;
    bool done;
#ifndef R3C_NOERRCHECK
    if ( (targetStr == NULL) || (numChars < 0) ) throw R3CERR_ILLEGALARGUMENT;
    if ( this->fileHandle == NULL ) throw R3CERR_IO_STREAMNOTOPEN;
#endif
    charsRead = 0;
    if ( numChars > 0 ) {
        targetStr->clear();
        done = false;
        while ( !done ) {
            curChar = getc(this->fileHandle);
            if ( ferror(this->fileHandle) != 0 ) throw R3CERR_IO_EXCEPTION;
            if ( curChar == EOF ) {
                if ( charsRead == 0 ) charsRead = EOF;
                done = true;
            } else {
                if ( curChar != 0 ) {
                    targetStr->append((char)curChar);
                    charsRead++;
                    if ( charsRead >= numChars ) done = true;
                }
            }
        }
    }
    return( charsRead );
}

int R3CTextInputFile::readLine(R3CString *targetStr) {
    int charsRead;
    int curChar;
    bool done;
    bool pastNewlines;
#ifndef R3C_NOERRCHECK
    if ( targetStr == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( this->fileHandle == NULL ) throw R3CERR_IO_STREAMNOTOPEN;
#endif
    charsRead = 0;
    done = false;
    pastNewlines = false;
    while ( !done ) {
        curChar = getc(this->fileHandle);
        if ( ferror(this->fileHandle) != 0 ) throw R3CERR_IO_EXCEPTION;
        if ( curChar == EOF ) {
            if ( charsRead == 0 ) charsRead = EOF;
            done = true;
        } else {
            if (
                (curChar == '\0') || 
                (strchr(R3C_STR_NEWLINE, curChar) != NULL)
            ) {
                if ( pastNewlines ) done = true;
            } else {
                pastNewlines = true;
                targetStr->append((char)curChar);
                charsRead++;
            }
        }
    }
    return( charsRead );
}


// *** CLOSE FILE *** //

void R3CTextInputFile::close() {
    int closeResult;
#ifndef R3C_NOERRCHECK
    if ( this->fileHandle == NULL ) throw R3CERR_IO_STREAMNOTOPEN;
#endif
    closeResult = fclose(this->fileHandle);
    this->fileHandle = NULL;
    if ( closeResult == EOF ) throw R3CERR_IO_EXCEPTION;
}
