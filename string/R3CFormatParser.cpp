
#include "../includes/r3c.hpp"
#include "../includes/r3c-string.hpp"

#include <stdio.h>


// *** FORMAT PIECES *** //

struct R3CFormatPiece {
	int startPos;
	int charCount;
	int dataType;
};


// *** CONSTRUCTION *** //

// Creates a new format parser object.
R3CFormatParser::R3CFormatParser() :
    formatString(NULL),
    piecesAlloc(16),
    piecesSet(0),
    pieces(NULL),
    stringDelimiter('\0')
{
    this->pieces = new R3CFormatPiece [16];
}


// *** DESTRUCTION *** //

// Destructor.
R3CFormatParser::~R3CFormatParser() {
    if ( this->pieces != NULL ) delete[] this->pieces;
}


// *** PARSE A FORMAT STRING *** //

// Ensures the pieces array is large enough to handle the current format
// string.
void R3CFormatParser::ensurePieceCapacity() {
    int oldPiecesAlloc;
    R3CFormatPiece* oldPieces;
    size_t piecesSize;

    // Check if the number of pieces set exceeds the number allocated
    if ( this->piecesSet >= this->piecesAlloc ) {
        // Keep track of the old piece data
        oldPiecesAlloc = this->piecesAlloc;
        oldPieces = this->pieces;

        // Allocate new pieces
        this->piecesAlloc = this->piecesAlloc << 1;
        this->pieces = new R3CFormatPiece [this->piecesAlloc];

        // Copy the old pieces into the new pieces
        piecesSize = oldPiecesAlloc * sizeof(R3CFormatPiece);
        memcpy(this->pieces, oldPieces, piecesSize);

        // Destroy the old piece pointers
        delete[] oldPieces;
    }
}

// Passes over all format flag characters in the format string.
const char* R3CFormatParser::passFlags(const char* strPtr) {
    const char* result;
    result = r3cStrPassChars(strPtr, "-+# ");
    return( result );
}

// Passes over all numeric characters (0 through 9) in the format string.
const char* R3CFormatParser::passNumber(const char* strPtr) {
    const char* result;
    result = r3cStrPassChars(strPtr, '0', '9');
    return( result );
}

// Passes over the precision portion of a numeric format entry in the format
// string.
const char* R3CFormatParser::passPrecision(const char* strPtr) {
    const char* result;
    result = strPtr;
    if ( *result == '.' ) {
        result++;
        result = this->passNumber(strPtr);
    }
    return( result );
}

// Passes over the size portion of a format entry in the format string.
const char* R3CFormatParser::passSize(const char* strPtr) {
    const char* result;
    result = strPtr;
    if ( *result == 'l' ) result++;
    return( result );
}

// Passes over all conversion characters within a format entry in the format
// string.
const char* R3CFormatParser::passConversion(const char* strPtr) {
    const char* result;
    result = strPtr + 1;
    result = this->passFlags(result);
    result = this->passNumber(result);
	result = this->passPrecision(result);
	result = this->passSize(result);
    return( result );
}

// Resolves the type of format entry for the given character.
int R3CFormatParser::resolveFormatType(char curChar) {
	int result;
	switch ( curChar ) {
		case 'd':
		case 'i':
		case 'c':
		case 'C':
		case 'u':
		case 'x':
		case 'X':
		case 'o':
			result = R3C_FORMAT_INT;
			break;
		case 'f':
		case 'g':
		case 'G':
		case 'e':
		case 'E':
			result = R3C_FORMAT_FLOAT;
			break;
		case 's':
			result = R3C_FORMAT_STRING;
			break;
		case 'p':
			result = R3C_FORMAT_POINTER;
			break;
		default:
			result = R3C_FORMAT_LITERAL;
	}
	return( result );
}

// Parses the given format string into its component parts.
void R3CFormatParser::parse(const char *formatString) {
	const char* strPtr;
	const char* percentPtr;
	const char* conversionPtr;
    R3CFormatPiece* currPiece;
	bool done;

#ifndef R3C_NOERRCHECK
    if ( formatString == NULL ) throw R3CERR_ILLEGALARGUMENT;
#endif

	this->formatString = formatString;
	strPtr = formatString;
	this->piecesSet = 0;
	done = false;
    while ( !done ) {
        // Find the next format piece
        percentPtr = strchr(strPtr, '%');
        if ( percentPtr != strPtr ) {
            // Create a literal string format piece up to the next % character
            this->ensurePieceCapacity();
            currPiece = this->pieces + this->piecesSet;
            currPiece->startPos = (int)(strPtr - formatString);
            currPiece->dataType = R3C_FORMAT_LITERAL;
            if ( percentPtr == NULL ) {
                // There is no % character, so this goes to the end
                currPiece->charCount = (int)strlen(strPtr);
                done = true;
            } else {
                currPiece->charCount = (int)(percentPtr - strPtr);
                strPtr = percentPtr;
            }

            // Move to the next piece
            this->piecesSet++;

        } else {
            // Create a format piece for this conversion
            conversionPtr = this->passConversion(strPtr);
            this->ensurePieceCapacity();
            currPiece = this->pieces + this->piecesSet;
            currPiece->startPos = (int)(strPtr - formatString);
            currPiece->dataType = this->resolveFormatType(*conversionPtr);
            currPiece->charCount = (int)(conversionPtr - strPtr) + 1;
            if ( (*conversionPtr == '%') && (conversionPtr == (strPtr+1)) ) {
                currPiece->charCount--;
            }

            // Move to the next piece
            strPtr = conversionPtr + 1;
            this->piecesSet++;
            if ( *strPtr == '\0' ) done = true;
        }
    }
}


// *** RETRIEVE FORMAT PIECE INFORMATION *** //

// Returns the number of pieces in the format string.
int R3CFormatParser::getPieceCount() {
	return( this->piecesSet );
}

// Returns the type of the piece at the given index.
int R3CFormatParser::getPieceType(int index) {
#ifndef R3C_NOERRCHECK
    if ( (index < 0) || (index >= this->piecesSet) ) throw R3CERR_OUTOFRANGE;
#endif
	return( this->pieces[index].dataType );
}


// *** APPEND FORMAT CONVERSIONS TO A STRING *** //

// Sets formatter to the format string represented by the piece at the given
// index.
void R3CFormatParser::setFormatter(int index) {
    R3CFormatPiece* curPiece;
    const char* formatStrPtr;
    curPiece = this->pieces + index;
    formatStrPtr = this->formatString + curPiece->startPos;
    strncpy(this->formatter, formatStrPtr, curPiece->charCount);
    this->formatter[curPiece->charCount] = '\0';
}

// Retrieves the delimiter character to output for string conversion.
char R3CFormatParser::getStringDelimiter() {
    return( this->stringDelimiter );
}

// Sets the delimiter character to output for string conversion.
void R3CFormatParser::setStringDelimiter(char delimiter) {
    this->stringDelimiter = delimiter;
}

// Appends the literal string piece at the given index.
void R3CFormatParser::appendLiteral(R3CString *str, int index) {
	R3CFormatPiece* curPiece;
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( (index < 0) || (index >= this->piecesSet) ) throw R3CERR_OUTOFRANGE;
    if ( this->pieces[index].dataType != R3C_FORMAT_LITERAL ) {
        throw R3CERR_STR_BADFORMATPIECE;
    }
#endif
	curPiece = this->pieces + index;
	str->append(this->formatString, curPiece->startPos, curPiece->charCount);
}

// Appends the integer conversion piece at the given index.
void R3CFormatParser::appendInt(R3CString *str, int index, long int value) {
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( (index < 0) || (index >= this->piecesSet) ) throw R3CERR_OUTOFRANGE;
    if ( this->pieces[index].dataType != R3C_FORMAT_INT ) {
        throw R3CERR_STR_BADFORMATPIECE;
    }
#endif
    this->setFormatter(index);
    sprintf(this->formatResult, this->formatter, value);
	str->append(this->formatResult);
}

// Appends the floating-point conversion piece at the given index.
void R3CFormatParser::appendFloat(
    R3CString *str, int index, long double value
) {
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( (index < 0) || (index >= this->piecesSet) ) throw R3CERR_OUTOFRANGE;
    if ( this->pieces[index].dataType != R3C_FORMAT_FLOAT ) {
        throw R3CERR_STR_BADFORMATPIECE;
    }
#endif
    this->setFormatter(index);
    sprintf(this->formatResult, this->formatter, value);
	str->append(this->formatResult);
}

// Appends the string conversion piece at the given index.
void R3CFormatParser::appendString(
    R3CString *str, int index, const char* value
) {
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( (index < 0) || (index >= this->piecesSet) ) throw R3CERR_OUTOFRANGE;
    if ( this->pieces[index].dataType != R3C_FORMAT_STRING ) {
        throw R3CERR_STR_BADFORMATPIECE;
    }
#endif
    if ( this->stringDelimiter == '\0' ) {
        str->append(value);
    } else {
        if ( value != NULL ) {
            str->append(this->stringDelimiter);
            str->append(value);
            str->append(this->stringDelimiter);
        } else {
            str->append("NULL");
        }
    }
}

// Appends the pointer conversion piece at the given index.
void R3CFormatParser::appendPointer(
    R3CString *str, int index, const void* value
) {
#ifndef R3C_NOERRCHECK
    if ( str == NULL ) throw R3CERR_ILLEGALARGUMENT;
    if ( (index < 0) || (index >= this->piecesSet) ) throw R3CERR_OUTOFRANGE;
    if ( this->pieces[index].dataType != R3C_FORMAT_POINTER ) {
        throw R3CERR_STR_BADFORMATPIECE;
    }
#endif
    this->setFormatter(index);
    sprintf(this->formatResult, this->formatter, value);
    str->append(this->formatResult);
}
