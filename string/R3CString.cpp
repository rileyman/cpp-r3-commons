
#include "../includes/r3c.hpp"
#include "../includes/r3c-string.hpp"

#include <string.h>
#include <stdarg.h>


// *** CONSTRUCTION *** //

// Initializes the storage capacity.
void R3CString::initMaxLength(int capacity) {
	int remainder;
	this->maxLength = capacity;
	if ( this->maxLength < 127 ) {
		this->maxLength = 127;
	} else {
		remainder = this->maxLength % 64;
		this->maxLength += 64 - remainder - 1;
	}
}

// Creates a new empty string.
R3CString::R3CString() :
    str(NULL),
    curLength(0),
    maxLength(127)
{
    this->str = new char [128];
    this->str[0] = '\0';
}

// Creates a new empty string, with the given storage capacity.
R3CString::R3CString(int capacity) :
    str(NULL),
    curLength(0),
    maxLength(127)
{
#ifndef R3C_NOERRCHECK
    if ( capacity < 0 ) throw R3CERR_ILLEGALARGUMENT;
#endif
    this->str = new char [this->maxLength + 1];
    this->str[0] = '\0';
}

// Creates a new string, copied from the source character string.
R3CString::R3CString(const char* sourceStr) :
    str(NULL),
    curLength(0),
    maxLength(127)
{
    if ( sourceStr != NULL ) {
        this->curLength = (int)strlen(sourceStr);
        this->initMaxLength(this->curLength);
    }
    this->str = new char [this->maxLength + 1];
    if ( sourceStr != NULL ) {
        strcpy(this->str, sourceStr);
    } else {
        this->str[0] = '\0';
    }
}

// Creates a new string, copied from the source string, with the given storage
// capacity.
R3CString::R3CString(const char* sourceStr, int capacity) :
    str(NULL),
    curLength(0),
    maxLength(capacity)
{
#ifndef R3C_NOERRCHECK
    if ( capacity < 0 ) throw R3CERR_ILLEGALARGUMENT;
#endif
    if ( sourceStr != NULL ) {
        this->curLength = (int)strlen(sourceStr);
    }
    if ( capacity < this->curLength ) {
        this->maxLength = this->curLength;
    }
    this->str = new char [this->maxLength + 1];
    if ( sourceStr != NULL ) {
        strcpy(this->str, sourceStr);
    } else {
        this->str[0] = '\0';
    }
}

// Creates a new string, copied from the source string.
R3CString::R3CString(R3CString *sourceStr) :
    str(NULL),
    curLength(0),
    maxLength(0)
{
#ifndef R3C_NOERRCHECK
    if ( sourceStr == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
    this->str = new char [sourceStr->maxLength + 1];
    strcpy(this->str, sourceStr->str);
    this->curLength = sourceStr->curLength;
    this->maxLength = sourceStr->maxLength;
}


// *** DESTRUCTION *** //

// Destructor.
R3CString::~R3CString() {
    if ( this->str != NULL ) delete[] this->str;
}


// *** RETRIEVE STRING INFORMATION *** //

// Returns the underlying character string.
char* R3CString::getChars() {
    return( this->str );
}

// Returns the length of this string, not including the zero-terminator.
int R3CString::getLength() {
	return( this->curLength );
}

// Returns the current storage capacity.
int R3CString::getCapacity() {
	return( this->maxLength );
}


// *** FINDING SUB-STRINGS *** //

// Returns the character at the given character position.
char R3CString::getCharAt(int pos) {
#ifndef R3C_NOERRCHECK
    if ( (pos < 0) || (pos >= this->curLength) ) throw R3CERR_OUTOFRANGE;
#endif
    return( this->str[pos] );
}

// Finds the first occurrence of the given character.
int R3CString::find(char charToFind) {
	char* charPtr;
    int result;
	charPtr = strchr(this->str, charToFind);
	if ( charPtr == NULL ) return( -1 );
    result = (int)(charPtr - this->str);
	return( result );
}

// Finds the first occurrence of the given sub-string.
int R3CString::find(const char *strToFind) {
	char* strPtr;
    int result;
#ifndef R3C_NOERRCHECK
    if ( strToFind == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
	strPtr = strstr(this->str, strToFind);
	if ( strPtr == NULL ) return( -1 );
    result = (int)(strPtr - this->str);
	return( result );
}

// Finds the last occurrence of the given character.
int R3CString::findReverse(char charToFind) {
	register char* charPtr;
	register char curChar;
	bool done;
    int result;
	if ( this->curLength < 1 ) return( -1 );
	charPtr = this->str + this->curLength - 1;
	done = false;
	while ( !done ) {
		curChar = *charPtr;
		if ( curChar == charToFind ) {
            result = (int)(charPtr - this->str);
			return( result );
		} else {
			if ( charPtr == this->str ) {
				done = true;
			} else {
				charPtr--;
			}
		}
	}
	return( -1 );
}


// *** COMPARING STRINGS *** //

// Compares this string to the given character string.
int R3CString::compare(const char* str) {
    int result;
	if ( str == NULL ) {
		result = ( (this->curLength == 0) ? 0 : 1 );
	} else {
		result = strcmp(this->str, str);
	}
    return( result );
}

// Compares this string to the given string.
int R3CString::compare(R3CString *str) {
    int result;
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
    result = strcmp(this->str, str->str);
    return( result );
}

// Checks if this string matches the given filename pattern.
bool R3CString::pathMatch(const char* pattern) {
    return( r3cPathMatch(this->str, pattern) );
}


// *** UPDATE STRING *** //

// Ensures the storage capacity can handle a string of the given length.
void R3CString::ensureCapacity(int newLength) {
	int remainder;
	char* oldPtr;
	if ( newLength <= this->maxLength ) return;
	this->maxLength = this->maxLength << 1;
	if ( this->maxLength < newLength ) this->maxLength = newLength;
	remainder = this->maxLength % 16;
	this->maxLength += 16 - remainder - 1;
	oldPtr = this->str;
	this->str = new char[this->maxLength + 1];
	strcpy(this->str, oldPtr);
	delete[] oldPtr;
}

// Resets the length of the string, based on the actual character string
// stored.
int R3CString::resetLength() {
	this->curLength = (int)strlen(this->str);
	return( this->curLength );
}

// Replaces this string with the source character string.
int R3CString::set(const char* sourceStr) {
	int newStrLength;
	if ( sourceStr == NULL ) {
		this->clear();
	} else {
		newStrLength = (int)strlen(sourceStr);
		this->ensureCapacity(newStrLength);
		strcpy(this->str, sourceStr);
		this->curLength = newStrLength;
	}
    return( this->curLength );
}

// Replaces this string with the source string.
int R3CString::set(R3CString* sourceStr) {
#ifndef R3C_NOERRCHECK
    if ( sourceStr == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
	this->ensureCapacity(sourceStr->curLength);
	strcpy(this->str, sourceStr->str);
	this->curLength = sourceStr->curLength;
    return( this->curLength );
}

// Appends the given character to the end of this string.
int R3CString::append(char charToAppend) {
    register char* charPtr;
#ifndef R3C_NOERRCHECK
    if ( charToAppend == '\0' ) throw R3CERR_ILLEGALARGUMENT;
#endif
	this->ensureCapacity(this->curLength + 1);
	charPtr = this->str + this->curLength;
	*charPtr = charToAppend;
	charPtr++;
	*charPtr = '\0';
	this->curLength++;
    return( 1 );
}

// Appends the source character string to the end of this string.
int R3CString::append(const char* sourceStr) {
	int sourceStrLength;
	char* appendPtr;
	if ( sourceStr == NULL ) return( 0 );
	sourceStrLength = (int)strlen(sourceStr);
	this->ensureCapacity(this->curLength + sourceStrLength);
	appendPtr = this->str + this->curLength;
	strcpy(appendPtr, sourceStr);
	this->curLength += sourceStrLength;
    return( sourceStrLength );
}

// Appends charCount characters, starting at startPos, from sourceStr into
// this string.
int R3CString::append(const char* sourceStr, int startPos, int charCount) {
	int sourceStrLength;
	char* appendPtr;
	if ( sourceStr == NULL ) return( 0 );
	sourceStrLength = (int)strlen(sourceStr);
    if ( (startPos + charCount) > sourceStrLength ) {
        charCount = sourceStrLength - startPos;
    }
	this->ensureCapacity(this->curLength + charCount);
	appendPtr = this->str + this->curLength;
	strncpy(appendPtr, sourceStr + startPos, charCount);
	appendPtr += charCount;
	*appendPtr = '\0';
	this->curLength += charCount;
    return( charCount );
}

// Appends the source string to the end of this string.
int R3CString::append(R3CString* sourceStr) {
	register int sourceStrLength;
    register char* appendPtr;
#ifndef R3C_NOERRCHECK
    if ( sourceStr == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
    sourceStrLength = sourceStr->curLength;
	this->ensureCapacity(this->curLength + sourceStrLength);
	appendPtr = this->str + this->curLength;
	strcpy(appendPtr, sourceStr->str);
	this->curLength += sourceStrLength;
    return( sourceStrLength );
}

// Appends the formatted string to the end of this string.
int R3CString::appendf(const char* formatString, ...) {
	R3CFormatParser formatter;
	va_list varArgs;
	int loop;
    if ( formatString == NULL ) return( 0 );
	formatter.parse(formatString);
	va_start(varArgs, formatString);
    for ( loop = 0; loop < formatter.getPieceCount(); loop++ ) {
        switch ( formatter.getPieceType(loop) ) {
            case R3C_FORMAT_LITERAL:
                formatter.appendLiteral(this, loop);
                break;
            case R3C_FORMAT_INT:
                formatter.appendInt(
                    this, loop, va_arg(varArgs, long int));
                break;
            case R3C_FORMAT_DOUBLE:
                formatter.appendFloat(
                    this, loop, va_arg(varArgs, long double));
                break;
            case R3C_FORMAT_STRING:
                formatter.appendString(
                    this, loop, va_arg(varArgs, const char*));
                break;
            case R3C_FORMAT_POINTER:
                formatter.appendPointer(
                    this, loop, va_arg(varArgs, const void*));
                break;
        } // TODO: test whether we need to have separate types for int vs. long int; and double vs. long double
    }
    va_end(varArgs);
    // TODO: determine # chars appended
    return( 0 );
}

// Inserts the given character at the given position in this string.
int R3CString::insert(int pos, char charToInsert) {
	register char* insertPtr;
#ifndef R3C_NOERRCHECK
	if ( charToInsert == '\0' ) throw R3CERR_ILLEGALARGUMENT;
	if ( (pos < 0) || (pos > this->curLength) ) throw R3CERR_OUTOFRANGE;
#endif
	this->ensureCapacity(this->curLength + 1);
	insertPtr = this->str + pos;
	memmove(insertPtr+1, insertPtr, this->curLength - pos + 1);
	*insertPtr = charToInsert;
	this->curLength++;
    return( 1 );
}

// Inserts the source character string at the given position in this string.
int R3CString::insert(int pos, const char* sourceStr) {
	register int sourceStrLen;
	register char* insertPtr;
    int charsToMove;
#ifndef R3C_NOERRCHECK
	if ( (pos < 0) || (pos > this->curLength) ) throw R3CERR_OUTOFRANGE;
#endif
	if ( sourceStr == NULL ) return( 0 );
	sourceStrLen = (int)strlen(sourceStr);
	this->ensureCapacity(this->curLength + sourceStrLen);
	insertPtr = this->str + pos;
    charsToMove = this->curLength - pos + 1;
	memmove(insertPtr + sourceStrLen, insertPtr, charsToMove);
	strncpy(insertPtr, sourceStr, sourceStrLen);
	this->curLength += sourceStrLen;
    return( sourceStrLen );
}

// Inserts charCount characters, starting at startPos, from sourceStr into
// this string at pos.
int R3CString::insert(
    int pos, const char* sourceStr, int startPos, int charCount
) {
	register int sourceStrLen;
	register char* insertPtr;
    int charsToMove;
#ifndef R3C_NOERRCHECK
	if ( (pos < 0) || (pos > this->curLength) ) throw R3CERR_OUTOFRANGE;
    if ( startPos < 0 ) throw R3CERR_ILLEGALARGUMENT;
#endif
	if ( sourceStr == NULL ) return( 0 );
	sourceStrLen = (int)strlen(sourceStr);
    if ( startPos >= sourceStrLen ) return( 0 );
    if ( (startPos + charCount ) > sourceStrLen ) {
        charCount = sourceStrLen - startPos;
    }
	this->ensureCapacity(this->curLength + charCount);
	insertPtr = this->str + pos;
    charsToMove = this->curLength - pos + 1;
	memmove(insertPtr + charCount, insertPtr, charsToMove);
	strncpy(insertPtr, sourceStr + startPos, charCount);
	this->curLength += charCount;
    return( charCount );
}

// Inserts the source string into this string.
int R3CString::insert(int pos, R3CString* sourceStr) {
    register char* insertPtr;
    int charsToMove;
#ifndef R3C_NOERRCHECK
    if ( (pos < 0) || (pos > this->curLength) ) throw R3CERR_OUTOFRANGE;
    if ( sourceStr == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
	this->ensureCapacity(this->curLength + sourceStr->curLength);
	insertPtr = this->str + pos;
    charsToMove = this->curLength - pos + 1;
	memmove(insertPtr + sourceStr->curLength, insertPtr, charsToMove);
	strncpy(insertPtr, sourceStr->str, sourceStr->curLength);
	this->curLength += sourceStr->curLength;
    return( sourceStr->curLength );
}

// Removes the character at the given position from this string.
int R3CString::deleteCharAt(int pos) {
    register char* deletePtr;
#ifndef R3C_NOERRCHECK
	if ( (pos < 0) || (pos >= this->curLength) ) throw R3CERR_OUTOFRANGE;
#endif
	deletePtr = this->str + pos;
	memmove(deletePtr, deletePtr + 1, this->curLength - pos + 1);
	this->curLength--;
    return( 1 );
}

// Removes all characters in this string between startPos and endPos,
// inclusive.
int R3CString::deleteChars(int startPos, int endPos) {
	register int deleteLength;
	register char* deletePtr;
    int charsToShift;
#ifndef R3C_NOERRCHECK
    if ( (startPos < 0) || (endPos > this->curLength) ) {
        throw R3CERR_OUTOFRANGE;
    }
#endif
	if ( endPos <= startPos ) return( 0 );
	deleteLength = endPos - startPos;
	deletePtr = this->str + startPos;
	charsToShift = this->curLength - startPos - deleteLength + 1;
	memmove(deletePtr, deletePtr + deleteLength, charsToShift);
	this->curLength -= deleteLength;
    return( deleteLength );
}

// Clears all characters from this string.
int R3CString::clear() {
    register int deleteLength;
    deleteLength = this->curLength;
    this->str[0] = '\0';
    this->curLength = 0;
    return( deleteLength );
}


// *** TRIM WHITESPACE *** //

// Left-trims this string.
int R3CString::trimLeft(const char* trimChars) {
	register char* curPtr;
	register int result;
#ifndef R3C_NOERRCHECK
    if ( trimChars == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif
	if ( this->curLength == 0 ) return( 0 );
	curPtr = this->str;
	while ( strchr(trimChars, *curPtr) != NULL ) {
		curPtr++;
	}
	result = 0;
	if ( curPtr > this->str ) {
		result = (int)(curPtr - this->str);
        memmove(this->str, curPtr, this->curLength - result + 1);
        this->curLength -= result;
	}
	return( result );
}
