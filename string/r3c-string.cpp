
#include "../includes/r3c.hpp"
#include "../includes/r3c-string.hpp"

#include <string.h>


// *** EXCEPTIONS *** //

const char* R3CERR_STR_BADFORMATPIECE = "R3CERR_STR_BADFORMATPIECE";

const char* R3CERR_STR_EMPTYSTACK = "R3CERR_STR_EMPTYSTACK";


// *** CONSTANTS *** //

const char* R3C_STR_EMPTY = "";

const char* R3C_STR_SPACE = " ";

const char* R3C_STR_LINESPACE = " \t";

const char* R3C_STR_NEWLINE = "\n\r\v\f";

const char* R3C_STR_WHITESPACE = " \t\n\r\v\f";


// *** COMMON STRING FUNCTIONS *** //

// Converts the given character string to upper-case.
int r3cStrUpper(char* str) {
	register char curChar;
	register char* curPtr;
    register int result;
    result = 0;
	if ( str != NULL ) {
		curPtr = str;
		curChar = *curPtr;
		while ( curChar != 0 ) {
			if ( (curChar >= 97) && (curChar <= 122) ) {
				(*curPtr) -= 32;
                result++;
			}
			curPtr++;
			curChar = *curPtr;
		}
	}
    return( result );
}

// Converts the given character string to lower-case.
int r3cStrLower(char* str) {
	register char curChar;
	register char* curPtr;
    register int result;
    result = 0;
	if ( str != NULL ) {
		curPtr = str;
		curChar = *curPtr;
		while ( curChar != 0 ) {
			if ( (curChar >= 65) && (curChar <= 90) ) {
				(*curPtr) += 32;
                result++;
			}
			curPtr++;
			curChar = *curPtr;
		}
	}
    return( result );
}

// Left-trims the given character string.
int r3cStrLTrim(char* str, const char* trimChars) {
	char* curPtr;
	int strLength, result;

    // If a NULL or empty string, return 0.
	if ( (str == NULL) || (trimChars == NULL) ) return( 0 );
	curPtr = str;
	if ( (*curPtr) == '\0' ) return( 0 );

    // Pass over characters that should be trimmed.
	while ( strchr(trimChars, *curPtr) != NULL ) {
		curPtr++;
	}

    // If any characters were passed over, move the remaining string (and its
    // null-terminator) to the front.
	result = 0;
	if ( curPtr > str ) {
		result = (int)(curPtr - str);
		strLength = (int)strlen(curPtr);
		memmove(str, curPtr, strLength + 1);
	}

    // Return the number of characters trimmed.
	return( result );
}

// Right-trims the given character string.
int r3cStrRTrim(char* str, const char* trimChars) {
	char* curPtr;
	char* endPtr;
	int strLength, result;
	char curChar;
	bool done;

    // If a NULL or empty string, return 0.
	if ( (str == NULL) || (trimChars == NULL) ) return( 0 );
	strLength = (int)strlen(str);
	if ( strLength == 0 ) return( 0 );

    // Starting with the last character, and moving backwards, pass over
    // characters that should be trimmed.
	curPtr = endPtr = str + strLength - 1;
	done = false;
	while ( !done ) {
		curChar = *curPtr;
		if ( strchr(trimChars, curChar) != NULL ) {
			curPtr--;
			if ( curPtr < str ) done = true;
		} else {
			done = true;
		}
	}

    // If any characters were passed over, terminate the string.
	result = 0;
	if ( curPtr < endPtr ) {
		curPtr[1] = 0;
		result = (int)(endPtr - curPtr);
	}

    // Return the number of characters trimmed.
	return( result );
}

// Fully trims the given character string.
int r3cStrTrim(char* str, const char* trimChars) {
	int strLength, charsTrimmed, result;
	char* curPtr;
	char* endPtr;
	char curChar;
	bool done;

    // If a NULL or empty string, return 0.
	if ( (str == NULL) || (trimChars == NULL) ) return( 0 );
	strLength = (int)strlen(str);
	if ( strLength == 0 ) return( 0 );

    // Starting with the last character, and moving backwards, pass over
    // characters that should be trimmed.
	result = 0;
	curPtr = endPtr = str + strLength - 1;
	done = false;
	while ( !done ) {
		curChar = *curPtr;
		if ( strchr(trimChars, curChar) ) {
			curPtr--;
			if ( curPtr < str ) done = true;
		} else {
			done = true;
		}
	}

    // If any characters were passed over, terminate the string.
	if ( curPtr < endPtr ) {
		curPtr[1] = 0;
		result = (int)(endPtr - curPtr);
		strLength -= result;
	}

    // Pass over characters that should be trimmed.
	curPtr = str;
	if ( (*curPtr) != '\0' ) {
		while ( strchr(trimChars, *curPtr) != NULL ) {
			curPtr++;
		}
	}

    // If any characters were passed over, move the remaining string (and its
    // null-terminator) to the front.
	if ( curPtr > str ) {
		charsTrimmed = (int)(curPtr - str);
		result += charsTrimmed;
		memmove(str, curPtr, strLength - charsTrimmed + 1);
	}

    // Return the number of characters trimmed.
	return( result );
}

// Passes over the given passable character in the target character string,
// until another character is reached.
const char* r3cStrPassChar(const char* targetStr, char passChar) {
    register const char* result;
    register char curChar;
    if ( targetStr == NULL ) return( targetStr );
    result = targetStr;
    curChar = *result;
    while ( (curChar != passChar) && (curChar != '\0') ) {
        result++;
        curChar = *result;
    }
    return( result );
}

// Passes over the given passable character in the target character string;
// until another character is reached, or until charCount characters have been
// passed.
const char* r3cStrPassChar(
    const char* targetStr, char passChar, int charCount
) {
    register const char* result;
    register char curChar;
    register int charLoop;
#ifndef R3C_NOERRCHECK
    if ( charCount < 0 ) throw R3CERR_ILLEGALARGUMENT;
#endif
    if ( (targetStr == NULL) || (charCount == 0) ) return( targetStr );
    result = targetStr;
    curChar = *result;
    charLoop = 0;
    while (
        (curChar != passChar) && (curChar != '\0') &&
        (charLoop < charCount)
    ) {
        result++;
        curChar = *result;
        charLoop++;
    }
    return( result );
}

// Passes over all characters in the target character string that match the
// given passable characters.
const char* r3cStrPassChars(const char* targetStr, const char* passChars) {
    register const char* result;
    if ( (targetStr == NULL) || (passChars == NULL) ) return( targetStr );
    result = targetStr;
    while ( strchr(passChars, *result) != NULL ) {
        result++;
    }
    return( result );
}

// Passes over all characters in the target character string that match the
// given passable characters, or until charCount characters have been passed.
const char* r3cStrPassChars(
    const char* targetStr, const char* passChars, int charCount
) {
    register const char* result;
    register char curChar;
    register int charLoop;
#ifndef R3C_NOERRCHECK
    if ( charCount < 0 ) throw R3CERR_ILLEGALARGUMENT;
#endif
    if ( (targetStr == NULL) || (passChars == NULL) ) return( targetStr );
    result = targetStr;
    charLoop = 0;
    if ( passChars == NULL ) {
        while ( (*result != '\0') && (charLoop < charCount) ) {
            result++;
            charLoop++;
        }
    } else {
        curChar = *result;
        while (
            (strchr(passChars, curChar) != NULL) && (charLoop < charCount)
        ) {
            result++;
            charLoop++;
            curChar = *result;
        }
    }
    return( result );
}

// Passes over all characters in the target character string that are within
// the given passable character range (inclusive).
const char* r3cStrPassChars(
    const char* targetStr, char firstPassChar, char lastPassChar
) {
    register const char* result;
    register char curChar;
#ifndef R3C_NOERRCHECK
    if ( firstPassChar > lastPassChar ) throw R3CERR_ILLEGALARGUMENT;
#endif
    if ( targetStr == NULL ) return( targetStr );
    result = targetStr;
    curChar = *result;
    while (
        (curChar >= firstPassChar) && (curChar <= lastPassChar) &&
        (curChar != '\0')
    ) {
        result++;
        curChar = *result;
    }
    return( result );
}

// Passes over all characters in the target character string that are within
// the given passable character range (inclusive), or until charCount
// characters have been passed.
const char* r3cStrPassChars(
    const char* targetStr,
    char firstPassChar, char lastPassChar,
    int charCount
) {
    register const char* result;
    register char curChar;
    register int charLoop;
#ifndef R3C_NOERRCHECK
    if ( firstPassChar > lastPassChar ) throw R3CERR_ILLEGALARGUMENT;
    if ( charCount < 0 ) throw R3CERR_ILLEGALARGUMENT;
#endif
    if ( targetStr == NULL ) return( targetStr );
    result = targetStr;
    curChar = *result;
    charLoop = 0;
    while (
        (curChar >= firstPassChar) && (curChar <= lastPassChar) &&
        (curChar != '\0') &&
        (charLoop < charCount)
    ) {
        result++;
        curChar = *result;
        charLoop++;
    }
    return( result );
}

// Passes over all characters in the target character string that are within
// any of the given passable character ranges (each inclusive).
const char* r3cStrPassChars(
    const char* targetStr,
    const char passGroups[][2], int groupCount
) {
    const char* result;
    int groupLoop;
    char curChar;
    bool groupsDone, done;

#ifndef R3C_NOERRCHECK
    // Check for illegal arguments
    if ( groupCount < 0 ) throw R3CERR_ILLEGALARGUMENT;
    if ( passGroups != NULL ) {
        for ( groupLoop = 0; groupLoop < groupCount; groupLoop++ ) {
            if ( passGroups[groupLoop][0] > passGroups[groupLoop][1] ) {
                throw R3CERR_ILLEGALARGUMENT;
            }
        }
    }
#endif

    // Check if no characters need to be passed over
    if (
        (targetStr == NULL) ||
        (passGroups == NULL) ||
        (groupCount == 0)
    ) {
        return( targetStr );
    }

    // Pass over all characters in any of the passable character groups
    result = targetStr;
    done = false;
    while ( !done ) {
        curChar = *result;
        if ( curChar == '\0' ) {
            done = true;
        } else {
            groupLoop = 0;
            groupsDone = false;
            while ( !groupsDone && !done ) {
                if (
                    (curChar >= passGroups[groupLoop][0]) &&
                    (curChar <= passGroups[groupLoop][1])
                ) {
                    result++;
                    groupsDone = true;
                } else {
                    groupLoop++;
                    if ( groupLoop >= groupCount ) done = true;
                }
            }
        }
    }

    // Return a pointer to the next character
    return( result );
}

// Passes over all characters in the target character string that are within
// any of the given passable character ranges (each inclusive), or until
// charCount characters have been passed.
const char* r3cStrPassChars(
    const char* targetStr,
    const char passGroups[][2], int groupCount,
    int charCount
) {
    const char* result;
    int groupLoop, charLoop;
    char curChar;
    bool groupsDone, done;

#ifndef R3C_NOERRCHECK
    // Check for illegal arguments
    if ( groupCount < 0 ) throw R3CERR_ILLEGALARGUMENT;
    if ( passGroups != NULL ) {
        for ( groupLoop = 0; groupLoop < groupCount; groupLoop++ ) {
            if ( passGroups[groupLoop][0] > passGroups[groupLoop][1] ) {
                throw R3CERR_ILLEGALARGUMENT;
            }
        }
    }
    if ( charCount < 0 ) throw R3CERR_ILLEGALARGUMENT;
#endif

    // Check if no characters need to be passed over
    if (
        (targetStr == NULL) ||
        (passGroups == NULL) ||
        (groupCount == 0)
    ) {
        return( targetStr );
    }

    // Pass over all characters in any of the passable character groups
    result = targetStr;
    charLoop = 0;
    done = false;
    while ( !done ) {
        curChar = *result;
        if ( (curChar == '\0') || (charLoop >= charCount) ) {
            done = true;
        } else {
            groupLoop = 0;
            groupsDone = false;
            while ( !groupsDone && !done ) {
                if (
                    (curChar >= passGroups[groupLoop][0]) &&
                    (curChar <= passGroups[groupLoop][1])
                ) {
                    result++;
                    charLoop++;
                    groupsDone = true;
                } else {
                    groupLoop++;
                    if ( groupLoop >= groupCount ) done = true;
                }
            }
        }
    }
    return( result );
}

// Passes over all characters in the target character string, until it finds
// the given reachable character.
const char* r3cStrReachChar(const char* targetStr, char reachChar) {
    register const char* result;
    register char curChar;
    if ( targetStr == NULL ) return( targetStr );
    result = targetStr;
    curChar = *result;
    while ( (curChar != reachChar) && (curChar != '\0') ) {
        result++;
        curChar = *result;
    }
    return( result );
}

// Passes over all characters in the target character string, until it finds
// one of the reachable characters.
const char* r3cStrReachChars(const char* targetStr, const char* reachChars) {
    register const char* result;
    if ( (targetStr == NULL) || (reachChars == NULL) ) return( targetStr );
    result = targetStr;
    while ( strchr(reachChars, *result) == NULL ) {
        result++;
    }
    return( result );
}

// Passes over all characters in the target character string, until it finds
// one of the reachable characters.
const char* r3cStrReachChars(
    const char* targetStr,
    char firstReachChar, char lastReachChar
) {
    register const char* result;
    register char curChar;
    bool done;
#ifndef R3C_NOERRCHECK
    if ( firstReachChar > lastReachChar ) throw R3CERR_ILLEGALARGUMENT;
#endif
    if ( targetStr == NULL ) return( targetStr );
    result = targetStr;
    done = false;
    while ( !done ) {
        curChar = *result;
        if (
            ( (curChar >= firstReachChar) && (curChar <= lastReachChar) ) ||
            (curChar == '\0')
        ) {
            done = true;
        } else {
            result++;
        }
    }
    return( result );
}

// Passes over all characters in the target character string, until it finds a
// character in any of the given reachable character ranges (each inclusive).
const char* r3cStrReachChars(
    const char* targetStr,
    const char reachGroups[][2], int groupCount
) {
    const char* result;
    int groupLoop;
    char curChar;
    bool groupsDone, done;

#ifndef R3C_NOERRCHECK
    // Check for illegal arguments
    if ( groupCount < 0 ) throw R3CERR_ILLEGALARGUMENT;
    if ( reachGroups != NULL ) {
        for ( groupLoop = 0; groupLoop < groupCount; groupLoop++ ) {
            if ( reachGroups[groupLoop][0] > reachGroups[groupLoop][1] ) {
                throw R3CERR_ILLEGALARGUMENT;
            }
        }
    }
#endif

    // Check if no characters need to be passed over
    if (
        (targetStr == NULL) ||
        (reachGroups == NULL) ||
        (groupCount == 0)
    ) {
        return( targetStr );
    }

    // Pass over all characters until a reachable character is found
    result = targetStr;
    done = false;
    while ( !done ) {
        curChar = *result;
        if ( curChar == '\0' ) {
            done = true;
        } else {
            groupLoop = 0;
            groupsDone = false;
            while ( !groupsDone && !done ) {
                if (
                    (curChar >= reachGroups[groupLoop][0]) &&
                    (curChar <= reachGroups[groupLoop][1])
                ) {
                    done = true;
                } else {
                    groupLoop++;
                    if ( groupLoop >= groupCount ) groupsDone = true;
                }
            }
            if ( !done ) result++;
        }
    }

    // Return a pointer to the next character
    return( result );
}

// Match ? wildcards between strings. This function is used internally.
// Returns true if charCount characters from str match the pattern.
bool r3cQmarkMatch(const char* str, const char* pattern, int charCount) {
	int loop;
	char patternChar, strChar;
	for ( loop = 0; loop < charCount; loop++ ) {
		patternChar = *pattern;
		strChar = *str;
		if ( (patternChar == 0) || (strChar == 0) ) return( false );
		if ( patternChar != '?' ) {
			if ( patternChar != strChar ) return( false );
		}
		pattern++;
		str++;
	}
	return( true );
}

// Finds the first character string from str that matches charCount characters
// in pattern.
int r3cFindQmarkMatch(const char* str, const char* pattern, int charCount) {
    const char* nonQmarkPatternPtr;
    int qmarkCount, nonQmarkCount;
    char matchChar;
    const char* searchStrPtr;
    int result;
    bool done;

    // Find first non-? character in str
    nonQmarkPatternPtr = r3cStrPassChar(str, '?', charCount);
    qmarkCount = (int)(nonQmarkPatternPtr - pattern);

    // If all ? characters, automatically match
    if ( qmarkCount == charCount ) return( charCount );

    // Match the first non-? character from pattern in str
    nonQmarkCount = charCount - qmarkCount;
    matchChar = *nonQmarkPatternPtr;
    if ( matchChar == '\0' ) return( 0 );
    searchStrPtr = str + qmarkCount;
    result = 0;
    do {
        searchStrPtr = strchr(searchStrPtr, matchChar);
        if ( searchStrPtr == NULL ) {
            done = true;
        } else {
            done = r3cQmarkMatch(
                searchStrPtr, nonQmarkPatternPtr, nonQmarkCount);
            if ( done ) {
                result = (int)(searchStrPtr - str) + charCount;
            }
        }
    } while ( !done );

    // Return the number of characters passed
    return( result );
}

// Checks if the given character string matches a filename pattern.
bool r3cPathMatch(const char* str, const char* pattern) {
	const char* strPtr;
    const char* patternPtr;
    const char* starPtr;
    int charCount;
    bool done, result;

#ifndef R3C_NOERRCHECK
    // Check for illegal arguments
	if ( (str == NULL) || (pattern == NULL) ) throw R3CERR_ILLEGALARGUMENT;
#endif

    strPtr = str;
    patternPtr = pattern;
    done = false;
    while ( !done ) {
        starPtr = strchr(patternPtr, '*');
        if ( starPtr == NULL ) {
            // Pattern contains no *, so match the remaining string
            charCount = (int)strlen(strPtr);
            result = r3cQmarkMatch(strPtr, patternPtr, charCount);
            done = true;
        } else if ( starPtr != patternPtr ) {
            // Pattern contains a * ahead, so match up to that *
            charCount = (int)(starPtr - patternPtr);
            result = r3cQmarkMatch(strPtr, patternPtr, charCount);
            if ( result ) {
                strPtr += charCount;
                patternPtr += charCount;
            } else {
                done = true;
            }
        } else {
            // We are at a *, so search ahead for matching non-star section
            patternPtr++;
            starPtr = r3cStrReachChar(patternPtr, '*');
            if ( starPtr != patternPtr ) {
                charCount = r3cFindQmarkMatch(
                    strPtr, patternPtr, (int)(starPtr - patternPtr));
                if ( charCount > 0 ) {
                    strPtr += charCount;
                    patternPtr += charCount;
                } else {
                    done = true;
                }
            }
        }
    }
    return( result );
}
