
/****************************************************************************
 * Riley's C++ Commons Library -- Strings
 * Copyright (C) 2010, Ryan Robinson
 *
 * Ryan Robinson
 * Web Site:  http://www.riley-man.com/
 * E-mail: riley@riley-man.com
 ****************************************************************************/

/*! \file r3c-string.hpp
 *  
 *  This is the include file for storing and managing strings.
 *  
 *  The "r3c" functions provide additional operations that can be performed
 *  against character strings, such as changing case, trimming (typically
 *  whitespace), and matching path patterns.
 *  
 *  Class R3CFormatParser provides a means to create functions that use
 *  C-style format strings.
 *  
 *  Class R3CString provides management of variable-length strings.  Class
 *  R3CPathString provides some convenience methods for managing directory
 *  path and filename strings.  Class R3CUnicode provides management of
 *  variable-length 16-bit unicode strings.
 *  
 *  Class R3CStringBlock provides a storage space for many constant strings,
 *  typically used when loading character strings from an external source.
 *  Class R3CStringBlockStack allows the storage space to be used in a
 *  first-in / last-out manner, at a group level.
 */

#ifndef _r3_commons_strings_HPP_
#define _r3_commons_strings_HPP_


// *** ADDITIONAL INCLUDES *** //

#include <string.h>


// *** DECLARATIONS *** //

// Exceptions

//! Exception indicating the format piece was not of the expected type.
extern const char* R3CERR_STR_BADFORMATPIECE;

//! Exception indicating the stack was empty, and could not be popped.
extern const char* R3CERR_STR_EMPTYSTACK;


// Constants

//! The empty string.
extern const char* R3C_STR_EMPTY;

//! A string containing a single space.
extern const char* R3C_STR_SPACE;

//! A string containing all whitespace characters that can occur on a single
//! line.
extern const char* R3C_STR_LINESPACE;

//! A string containing all whitespace characters that can cause a new line.
extern const char* R3C_STR_NEWLINE;

//! A string containing all whitespace characters.
extern const char* R3C_STR_WHITESPACE;


// Class List

class R3CFormatParser;
class R3CString;
class R3CPathString;
class R3CStringBlock;
class R3CStringBlockStack;
class R3CUnicode;


// Functions

/*! Converts the given character string to upper-case.
    
    \param str Character string.
    \return Number of characters converted to upper-case.
*/
int r3cStrUpper(char* str);

/*! Converts the given character string to lower-case.
    
    \param str Character string.
    \return Number of characters converted to lower-case.
*/
int r3cStrLower(char* str);

/*! Left-trims the given character string.

    \param str Character string.
    \param trimChars Characters that should be trimmed, typically
        R3C_STR_SPACE (to trim only spaces) or R3C_STR_LINESPACE (to trim
        spaces and tabs).
    \return Number of characters trimmed.
*/
int r3cStrLTrim(char* str, const char* trimChars);

/*! Right-trims the given character string.
    
    \param str Character string.
    \param trimChars Characters that should be trimmed, typically
        R3C_STR_SPACE (to trim only spaces) or R3C_STR_LINESPACE (to trim
        spaces and tabs).
    \return Number of characters trimmed.
*/
int r3cStrRTrim(char* str, const char* trimChars);

/*! Fully trims the given character string.
    
    \param str Character string.
    \param trimChars Characters that should be trimmed, typically
        R3C_STR_SPACE (to trim only spaces) or R3C_STR_LINESPACE (to trim
        spaces and tabs).
    \return Number of characters trimmed.
*/
int r3cStrTrim(char* str, const char* trimChars);

/*! Passes over the given passable character in the target character string,
    until another character is reached.

    \param targetStr Target character string.
    \param passChar Passable character.
    \return Pointer to the next character in targetStr that does not match
        the passable character.
*/
const char* r3cStrPassChar(const char* targetStr, char passChar);

/*! Passes over the given passable character in the target character string;
    until another character is reached, or until charCount characters have
    been passed.

    \param targetStr Target character string.
    \param passChar Passable character.
    \param charCount Maximum number of characters to pass.
    \return Pointer to the next character in targetStr that does not match
        the passable character, or to (targetStr + charCount), whichever is
        first.
    \throws R3CERR_ILLEGALARGUMENT If charCount is less than 0.
*/
const char* r3cStrPassChar(
    const char* targetStr, char passChar, int charCount);

/*! Passes over all characters in the target character string that match the
    given passable characters.  If passChars is NULL, no action is taken.

    \param targetStr Target character string.
    \param passChars Passable characters.
    \return Pointer to the next character in targetStr that does not match any
        of the passable characters.
*/
const char* r3cStrPassChars(const char* targetStr, const char* passChars);

/*! Passes over all characters in the target character string that match the
    given passable characters, or until charCount characters have been passed.

    \param targetStr Target character string.
    \param passChars Passable characters.
    \param charCount Maximum number of characters to pass.
    \return Pointer to the next character in targetStr that does not match any
        of the passable characters, or to (targetStr + charCount), whichever
        is first.
    \throws R3CERR_ILLEGALARGUMENT If charCount is less than 0.
*/
const char* r3cStrPassChars(
    const char* targetStr, const char* passChars, int charCount);

/*! Passes over all characters in the target character string that are within
    the given passable character range (inclusive).

    \param targetStr Target character string.
    \param firstPassChar First passable character.
    \param lastPassChar Last passable character.
    \return Pointer to the next character in targetStr that does not match any
        of the passable characters.
    \throws R3CERR_ILLEGALARGUMENT If firstPassChar is greater than
        lastPassChar.
*/
const char* r3cStrPassChars(
    const char* targetStr, char firstPassChar, char lastPassChar);

/*! Passes over all characters in the target character string that are within
    the given passable character range (inclusive), or until charCount
    characters have been passed.

    \param targetStr Target character string.
    \param firstPassChar First passable character.
    \param lastPassChar Last passable character.
    \param charCount Maximum number of characters to pass.
    \return Pointer to the next character in targetStr that does not match any
        of the passable characters, or to (targetStr + charCount), whichever
        is first.
    \throws R3CERR_ILLEGALARGUMENT If firstPassChar is greater than
        lastPassChar, or charCount is less than 0.
*/
const char* r3cStrPassChars(
    const char* targetStr,
    char firstPassChar, char lastPassChar,
    int charCount);

/*! Passes over all characters in the target character string that are within
    any of the given passable character ranges (each inclusive).  If
    passGroups is NULL, or groupCount is 0, no action is taken.

    \param targetStr Target character string.
    \param passGroups Array of passable character ranges.
    \param groupCount Number of passable character ranges.
    \return Pointer to the next character in targetStr that does not match any
        of the passable characters.
    \throws R3CERR_ILLEGALARGUMENT If the first character in any passGroup is
        greater than the second character; or groupCount is less than 0.
*/
const char* r3cStrPassChars(
    const char* targetStr, const char passGroups[][2], int groupCount);

/*! Passes over all characters in the target character string that are within
    any of the given passable character ranges (each inclusive), or until
    charCount characters have been passed.

    \param targetStr Target character string.
    \param passGroups Array of passable character ranges.
    \param groupCount Number of passable character ranges.
    \param charCount Maximum number of characters to pass.
    \return Pointer to the next character in targetStr that does not match any
        of the passable characters, or to (targetStr + charCount), whichever
        is first.
    \throws R3CERR_ILLEGALARGUMENT If the first character in any passGroup is
        greater than the second character; groupCount is less than 0; or
        charCount is less than 0.
*/
const char* r3cStrPassChars(
    const char* targetStr,
    const char passGroups[][2], int groupCount,
    int charCount);

/*! Passes over all characters in the target character string, until it finds
    the given reachable character.

    \param targetStr Target character string.
    \param reachChar Reachable character.
    \return Pointer to the next character in targetStr that matches the
        reachable character, or the null-terminator.
*/
const char* r3cStrReachChar(const char* targetStr, char reachChar);

/*! Passes over all characters in the target character string, until it finds
    one of the reachable characters.  If reachChars is NULL, no action is
    taken.

    \param targetStr Target character string.
    \param reachChars Reachable characters.
    \return Pointer to the next character in targetStr that matches any one of
        the reachable characters, or the null-terminator.
*/
const char* r3cStrReachChars(const char* targetStr, const char* reachChars);

/*! Passes over all characters in the target character string, until it finds
    a character in the given reachable character range (inclusive).

    \param targetStr Target character string.
    \param firstReachChar First reachable character.
    \param lastReachChar Last reachable character.
    \return Pointer to the next character in targetStr that matches any one of
        the reachable characters, or the null-terminator.
    \throws R3CERR_ILLEGALARGUMENT If firstReachChar is greater than
        lastReachChar.
*/
const char* r3cStrReachChars(
    const char* targetStr, char firstReachChar, char lastReachChar);

/*! Passes over all characters in the target character string, until it finds
    a character in any of the given reachable character ranges (each
    inclusive).  If reachGroups is NULL, or groupCount is 0, no action is
    taken.

    \param targetStr Target character string.
    \param reachGroups Array of reachable character ranges.
    \param groupCount Number of reachable character ranges.
    \return Pointer to the next character in targetStr that matches any one of
        the reachable characters, or the null-terminator.
    \throws R3CERR_ILLEGALARGUMENT If the first character in any reachGroup is
        greater than the second character; or groupCount is less than 0.
*/
const char* r3cStrReachChars(
    const char* targetStr, const char reachGroups[][2], int groupCount);

/*! Checks if the given character string matches a filename pattern.
    
    \param str Character string.
    \param pattern String containing the filename mattern.
    \return Flag indicating whether the character string matches the filename
        pattern.
    \throws R3CERR_ILLEGALARGUMENT If either str or pattern is NULL.
*/
bool r3cPathMatch(const char* str, const char* pattern);


// *** CLASS DEFINITIONS *** //

/* R3CFormatParser */

// Class-related constants

//! Type of format piece, indicating a literal string.
#define R3C_FORMAT_LITERAL 0
//! Type of format piece, indicating an integer or character.
#define R3C_FORMAT_INT 1
//! Type of format piece, indicating a floating-point number.
#define R3C_FORMAT_DOUBLE 2
//! Type of format piece, indicating a string.
#define R3C_FORMAT_STRING 3
//! Type of format piece, indicating a generic pointer.
#define R3C_FORMAT_POINTER 4

// Class-related data types

struct R3CFormatPiece;

// Class definition with doxygen comments

/*! Represents a C-style format string.
 *  This is used to build a formatted string from a C-style format string.
 *  Parse a new string by calling \ref parse.  Once the format string has been
 *  parsed, loop through the pieces of the string using the va_start, va_arg,
 *  and va_end macros:
 *  - Call \ref getPieceType to determine the current piece's data type.
 *  - Call the corresponding <b>append</b> method to append the piece to an
 *    R3CString object.
 *  .
 */
class R3CFormatParser {

// Member Variables

protected:

    //! Original format string.
    const char* formatString;

    //! Number of format pieces currently allocated in memory.
    int piecesAlloc;

    //! Number of format pieces in the original format string.
    int piecesSet;

    //! Array of format pieces.
    R3CFormatPiece* pieces;

    //! String delimiter character.
    char stringDelimiter;

    //! Current format string, used by appendPiece methods.
    char formatter[28];

    //! Current result of format conversion, used by appendPiece methods.
    char formatResult[96];


// Construction

public:
    
    //! Creates a new format parser object.
    R3CFormatParser();


// Destruction

public:

    //! Destructor.
    ~R3CFormatParser();


// Parse a format string

protected:

    //! Ensures the pieces array is large enough to handle the current format
    //! string.
    void ensurePieceCapacity();

    /*! Passes over all format flag characters in the format string.  Flag
        characters include the -, +, #, and space characters.

        \param strPtr Pointer to the current position in the format string.
        \return Pointer to the new position in the format string.
     */
    const char* passFlags(const char* strPtr);

    /*! Passes over all numeric characters (0 through 9) in the format string.

        \param strPtr Pointer to the current position in the format string.
        \return Pointer to the new position in the format string.
    */
    const char* passNumber(const char* strPtr);

    /*! Passes over the precision portion of a numeric format entry in the
        format string.  This passes over a dot character, followed by all
        numeric characters (0 through 9).

        \param strPtr Pointer to the current position in the format string.
        \return Pointer to the new position in the format string.
    */
    const char* passPrecision(const char* strPtr);

    /*! Passes over the size portion of a format entry in the format string.
        This includes the letter "l".

        \param strPtr Pointer to the current position in the format string.
        \return Pointer to the new position in the format string.
    */
    const char* passSize(const char* strPtr);

    /*! Passes over all conversion characters within a format entry in the
        format string.  This first passes by all flag characters, then all
        numeric characters, then the precision portion, then the size portion.
        The resulting pointer is located at the character that indicates the
        type of this format entry.

        \param strPtr Pointer to the current position in the format string.
        \return Pointer to the new position in the format string.
    */
    const char* passConversion(const char* strPtr);

    /*! Resolves the type of format entry for the given character.
        
        \param curChar Format character.
        \return Type of format entry, as identified by the corresponding
            R3C_FORMAT_ constant.
    */
    int resolveFormatType(char curChar);


public:

    /*! Parses the given format string into its component parts.
    
        \param formatString Format string.
    */
    void parse(const char* formatString);


// Retrieve format piece information

public:

    /*! Returns the number of pieces in the format string.

        \return Number of pieces in the format string.
    */
    int getPieceCount();

    /*! Returns the type of the piece at the given index, as identified by the
        corresponding R3C_FORMAT_ constant.
        
        \param index Piece index, where the first index is 0.
        \return Type of the given piece.
    */
    int getPieceType(int index);


// Append format conversions to a string

protected:

    /*! Sets \ref formatter to the format string represented by the piece at
        the given index.

        \param index Piece index, where the first index is 0.
    */
    void setFormatter(int index);

public:

    /*! Retrieves the delimiter character to output, used by the \ref
        appendString method.

        \return String delimiter character.
    */
    char getStringDelimiter();

    /*! Sets the delimiter character to output, used by the \ref appendString
        method.

        \param delimiter String delimiter character.
    */
    void setStringDelimiter(char delimiter);

    /*! Appends the piece at the given index, which is expected to be of type
        R3C_FORMAT_LITERAL.

        \param str Target string.
        \param index Piece index, where the first index is 0.
    */
    void appendLiteral(R3CString* str, int index);

    /*! Appends the piece at the given index, which is expected to be of type
        R3C_FORMAT_INT, using the given integer value as the replacement.

        \param str Target string.
        \param index Piece index, where the first index is 0.
        \param value Integer value.
    */
    void appendInt(R3CString *str, int index, long int value);

    /*! Appends the piece at the given index, which is expected to be of type
        R3C_FORMAT_FLOAT, using the given floating-point value as the
        replacement.

        \param str Target string.
        \param index Piece index, where the first index is 0.
        \param value Floating-point value.
    */
    void appendFloat(R3CString *str, int index, long double value);

    /*! Appends the piece at the given index, which is expected to be of type
        R3C_FORMAT_STRING, using the given character string as the
        replacement.
        
        If a delimiter has been set by a call to \ref setStringDelimiter, the
        string output will be wrapped by that delimiter.  If value is NULL,
        the string NULL is printed, without the delimiters.

        \param str Target string.
        \param index Piece index, where the first index is 0.
        \param value Pointer to a character string.
    */
    void appendString(R3CString *str, int index, const char* value);

    /*! Appends the piece at the given index, which is expected to be of type
        R3C_FORMAT_POINTER, using the given generic pointer as the
        replacement.

        \param str Target string.
        \param index Piece index, where the first index is 0.
        \param value Generic pointer.
    */
    void appendPointer(R3CString *str, int index, const void* value);

}; // end R3CFormatParser


/* R3CString */

// Class definition with doxygen comments

/*! Stores a dynamically allocated character string.
 *  String objects will expand in size as necessary.  Note that subsequent
 *  calls to \ref getChars are not guaranteed to return the same pointer.
 *
 *  The current implementation allocates storage in multiples of 128.  If a
 *  string is expected to grow larger than 128, it is good practice to specify
 *  the expected capacity during construction.
 */
class R3CString {

// Member Variables

protected:

    //! Character string.
    char* str;

    //! Current length of string, not including the zero-terminator.
    int curLength;

    //! Current maximum length of string, not including the zero-terminator.
    int maxLength;


// Construction

private:

    /*! Initializes the storage capacity.
        
        \param capacity New storage capacity.
    */
    void initMaxLength(int capacity);

public:

    //! Creates a new empty string.
    R3CString();

    /*! Creates a new empty string, with the given storage capacity.
        
        \param capacity Storage capacity.
        \throws R3CERR_ILLEGALARGUMENT If capacity is less than 1.
    */
    R3CString(int capacity);

    /*! Creates a new string, copied from the source character string.  If the
        source character string is NULL, the new string will be empty.
        
        \param sourceStr Source character string.
    */
    R3CString(const char* sourceStr);

    /*! Creates a new string, copied from the source string, with the given
        storage capacity.  If the source character string is NULL, the new
        string will be empty.

        \param sourceStr Source character string.
        \param capacity Storage capacity.
        \throws R3CERR_ILLEGALARGUMENT If capacity is less than the length of
            sourceStr, or less than 1.
    */
    R3CString(const char* sourceStr, int capacity);

    /*! Creates a new string, copied from the source string.

        \param sourceStr Source string.
        \throws R3CERR_ILLEGALARGUMENT If sourceStr is NULL.
    */
    R3CString(R3CString* sourceStr);


// Destruction

public:

    //! Destructor.
    ~R3CString();


// Retrieve String Information

public:

    /*! Returns the underlying character string.
        This method is available with the understanding that calling code
        should not perform write operations that affect the length of the
        string.  It is preferred that calling code use the returned character
        string for read-only purposes.
        
        \return Character string.
    */
    char* getChars();

    /*! Returns the length of this string, not including the zero-terminator.
        
        \return Length of this string.
    */
    int getLength();

    /*! Returns the current storage capacity.
        
        \return Current storage capacity.
    */
    int getCapacity();


// Finding Sub-Strings

public:

    /*! Returns the character at the given character position.

        \param pos Character position, where the first character is at
            position 0.
        \return Character.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the string length.
    */
    char getCharAt(int pos);

    /*! Finds the first occurrence of the given character.
        
        \param charToFind Character to find.
        \return Position of the given character, or -1 if the character was
            not found.
    */
    int find(char charToFind);

    /*! Finds the first occurrence of the given sub-string.
        
        \param strToFind Sub-string to find.
        \return Position of the given sub-string, or -1 if the sub-string was
            not found.
        \throws R3CERR_ILLEGALARGUMENT If strToFind is NULL.
    */
    int find(const char* strToFind);

    /*! Finds the last occurrence of the given character.

        \param charToFind Character to find.
        \return Position of the given character, or -1 if the character was
            not found.
    */
    int findReverse(char charToFind);


// Comparing Strings

public:

    /*! Compares this string to the given character string.  Note that NULL is
        treated like an empty string.

        \param str Character string to compare to.
        \return Value that is less than 0, equal to 0, or greater than 0; as
            the passed string is less than, equal to, or greater than this
            string.
    */
    int compare(const char* str);

    /*! Compares this string to the given string.

        \param str String to compare to.
        \return Value that is less than 0, equal to 0, or greater than 0; as
            the passed string is less than, equal to, or greater than this
            string.
        \throws R3CERR_ILLEGALARGUMENT If str is NULL.
    */
    int compare(R3CString* str);

public:
    
    /*! Checks if this string matches the given filename pattern.
        
        \param pattern Filename pattern.
        \return Flag indicating whether this string matches the given filename
            pattern.
        \throws R3CERR_ILLEGALARGUMENT If pattern is NULL.
    */
    bool pathMatch(const char* pattern);


// Update String

public:

    /*! Ensures the storage capacity can handle a string of the given length.
        This is called internally whenever an operation expands the length of
        the string.  It is provided as a public method in the rare case that
        calling code needs to modify the length of the string externally.

        \param newLength String length.
    */
    void ensureCapacity(int newLength);

    /*! Resets the length of the string, based on the actual character string
        stored.  This is provided as a public method in the rare case that
        calling code needs to modify the length of the string externally.
        
        \return Length of the string.
    */
    int resetLength();

    /*! Replaces this string with the source character string.  If the source
        character string is NULL, the string is cleared.
        
        \param sourceStr Source character string.
        \return Length of the string.
    */
    int set(const char* sourceStr);

    /*! Replaces this string with the source string.
        
        \param sourceStr Source string.
        \return Length of the string.
        \throws R3CERR_ILLEGALARGUMENT If sourceStr is NULL.
    */
    int set(R3CString* sourceStr);

    /*! Appends the given character to the end of this string.

        \param charToAppend Character to append.
        \return Number of characters appended (always 1).
        \throws R3CERR_ILLEGALARGUMENT If charToAppend is a null-terminator.
    */
    int append(char charToAppend);

    /*! Appends the source character string to the end of this string.  If the
        source character string is NULL, 0 is returned.

        \param sourceStr Source character string.
        \return Number of characters appended.
    */
    int append(const char* sourceStr);

    /*! Appends charCount characters, starting at startPos, from sourceStr
        into this string.  If sourceStr is NULL, 0 is returned.  If startPos
        is greater than the length of sourceStr, no characters are appended.
        Only characters up to the end of sourceStr are appended.

        \param sourceStr Source character string.
        \param startPos Starting character position.
        \param charCount Number of characters to append.
        \return Number of characters actually appended.
        \throws R3CERR_ILLEGALARGUMENT If startPos is less than 0.
    */
    int append(const char* sourceStr, int startPos, int charCount);

    /*! Appends the source string to the end of this string.
        
        \param sourceStr Source string.
        \return Number of characters appended.
        \throws R3CERR_ILLEGALARGUMENT If sourceStr is NULL.
    */
    int append(R3CString* sourceStr);

    /*! Appends the formatted string to the end of this string.
        
        \param formatString C-style format string.
        \param ... Format parameter replacements.
        \return Number of characters appended.
    */
    int appendf(const char* formatString, ...);

    /*! Inserts the given character at the given position in this string.
        
        \param pos Position to insert into this string.
        \param charToInsert Character to insert.
        \return Number of characters inserted (always 1).
        \throws R3CERR_ILLEGALARGUMENT If charToInsert is the null-terminator.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or
            equal to the length of this string.
    */
    int insert(int pos, char charToInsert);

    /*! Inserts the source character string at the given position in this
        string.  If the source character string is NULL, 0 is returned.

        \param pos Position to insert into this string.
        \param sourceStr Source character string.
        \return Number of characters inserted.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
    */
    int insert(int pos, const char* sourceStr);

    /*! Inserts charCount characters, starting at startPos, from sourceStr
        into this string at pos.  If sourceStr is NULL, 0 is returned.  If
        startPos is greater than the length of sourceStr, no characters are
        inserted.  Only characters up to the end of sourceStr are appended.

        \param pos Position to insert into this string.
        \param sourceStr Source character string.
        \param startPos Starting character position.
        \param charCount Number of characters to insert.
        \return Number of characters actually inserted.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
        \throws R3CERR_ILLEGALARGUMENT If startPos is less than 0.
    */
    int insert(int pos, const char* sourceStr, int startPos, int charCount);

    /*! Inserts the source string into this string.
        
        \param pos Position to insert into this string.
        \param sourceStr Source string.
        \return Number of characters actually inserted.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
        \throws R3CERR_ILLEGALARGUMENT If sourceStr is NULL.
    */
    int insert(int pos, R3CString* sourceStr);

    /*! Removes the character at the given position from this string.
        
        \param pos Position of character in this string.
        \return Number of characters deleted (always 1).
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
    */
    int deleteCharAt(int pos);

    /*! Removes all characters in this string between startPos and endPos,
        inclusive.

        \param startPos Starting position in this string.
        \param endPos Ending position in this string.
        \return Number of characters deleted.
        \throws R3CERR_OUTOFRANGE If startPos less than 0, or endPos is
            greater or equal to the length of this string.
    */
    int deleteChars(int startPos, int endPos);

    /*! Clears all characters from this string.
        
        \return Number of characters deleted.
    */
    int clear();


// Trim Whitespace

public:

    /*! Left-trims this string.
        
        \param trimChars Characters that should be trimmed, typically
            R3C_STR_SPACE (to trim only spaces) or R3C_STR_WHITESPACE (to trim
            spaces, tabs, carriage returns, newlines, and form feeds).
        \return Number of characters trimmed.
    */
    int trimLeft(const char* trimChars);

    /*! Right-trims this string.

        \param trimChars Characters that should be trimmed, typically
            R3C_STR_SPACE (to trim only spaces) or R3C_STR_LINESPACE (to trim
            spaces and tabs).
        \return Number of characters trimmed.
    */
    int trimRight(const char* trimChars);

    /*! Fully trims this string.

        \param trimChars Characters that should be trimmed, typically
            R3C_STR_SPACE (to trim only spaces) or or R3C_STR_LINESPACE (to
            trim spaces and tabs).
        \return Number of characters trimmed.
    */
    int trim(const char* trimChars);


// Manage Case

public:

    /*! Converts this string to upper-case.

        \return Number of characters converted to upper-case.
    */
    int toUpper();

    /*! Converts this string to lower-case.

        \return Number of characters converted to upper-case.
    */
    int toLower();


}; // end R3CString


/* R3CPathString */

// Class definition with doxygen comments

/*! Stores a dynamically allocated file or folder path string.
 *  If compiled under Windows, the path separator character is the backslash.
 *  Otherwise, it is the forward slash.
 */
class R3CPathString :
    public R3CString
{

// Construction

public:

    //! Creates a new empty file or folder path string.
    R3CPathString();

    /*! Creates a new empty file or folder path string, with the given storage
        capacity.

        \param capacity Storage capacity.
        \throws R3CERR_ILLEGALARGUMENT If capacity is less than 1.
    */
    R3CPathString(int capacity);

    /*! Creates a new file or folder path string, copied from the source
        character string.  If the source character string is NULL, the new
        string will be empty.
        
        \param sourceStr Source character string.
    */
    R3CPathString(const char* sourceStr);

    /*! Creates a new file or folder path string, copied from the source
        string.

        \param sourceStr Source character string.
        \throws R3CERR_ILLEGALARGUMENT If sourceStr is NULL.
    */
    R3CPathString(R3CString* sourceStr);

    /*! Creates a new file or folder path string, copied from the source
        directory path and filename character strings.

        \param dirPath Source directory path character string.
        \param filename Source filename character string.
    */
    R3CPathString(const char* dirPath, const char* filename);

    /*! Creates a new file or folder path string, copied from the source
        directory path and filename strings.

        \param dirPath Source directory path string.
        \param filename Source filename string.
    */
    R3CPathString(R3CString* dirPath, R3CString* filename);


// Update String

public:

    /*! Appends the source path character string to this string.

        \param path Source path character string.
        \param trailingSlash Flag indicating whether a trailing slash
            character must be appended, if the path does not already end in a
            slash.
        \throws R3CERR_ILLEGALARGUMENT If path is NULL.
    */
    void appendPath(const char* path, bool trailingSlash);

    /*! Appends the source path string to this string.

        \param path Source path string.
        \param trailingSlash Flag indicating whether a trailing slash
            character must be appended, if the path does not already end in a
            slash.
        \throws R3CERR_ILLEGALARGUMENT If path is NULL.
    */
    void appendPath(R3CString* path, bool trailingSlash);


// Retrieve Path Components

public:

    /*! Retrieves the number of folders that make up this path string.
        
        \return Number of folders that make up this path string.
    */
    int getFolderCount();

    /*! Retrieves the folder string at the given folder index within this path
        string.

        \param targetStr Target string.
        \param folderIndex Folder index.
    */
    void getFolder(R3CString* targetStr, int folderIndex);

    /*! Retrieves the full path string, not including the filename.

        \param targetStr Target string.
    */
    void getFolder(R3CString* targetStr);

    /*! Retrieves the filename from this path string.

        \param targetStr Target string.
    */
    void getFilename(R3CString* targetStr);

    
}; // end R3CPathString


/* R3CStringBlock */

// Class definition with doxygen comments

/*! Provides a large, expandable block of memory for storage of character
 *  strings.  This class can be used as a storage area for a large number of
 *  character strings that are expected to persist for some known lifetime.
 *  Once stored, the character strings are intended to be of a constant
 *  length (or some maximum length).
 *
 *  Note that if a string that is at least half the size of a single storage
 *  block size is added, it may be allocated its own storage block.
 *  
 *  Use of this class instead of many individual memory allocations can help
 *  to prevent memory fragmentation and allocation-related performance
 *  problems.
 */
class R3CStringBlock {

// Member Variables

protected:

    //! Number of bytes in each storage block.
    int bytesPerBlock;

    //! Number of pointers to storage blocks currently allocated.
    int memBlockAlloc;

    //! Array of pointers to storage blocks.  Each storage block is allocated
    //! if its pointer is not NULL.
    char** memBlock;

    //! Current storage block in which to allocate character strings.
    int currMemBlock;

    //! Pointer to the next available position in the current storage block.
    char* nextStrPtr;

    //! Number of bytes already used in the current storage block.
    int bytesUsedInBlock;


// Construction

private:

    /*! Initializes this string storage block.  A single storage block will be
        allocated of the given size.
        
        \param kbPerBlock Number of kilobytes per storage block.
    */
    void init(int kbPerBlock);

public:

    //! Creates a new string storage block.
    R3CStringBlock();

    /*! Creates a new string storage block.  Each new storage block will be
        allocated of the given size.

        \param kbPerBlock Number of kilobytes per storage block.
    */
    R3CStringBlock(int kbPerBlock);


// Destruction

public:

    //! Destructor.
    virtual ~R3CStringBlock();


// Manage Strings

protected:

    /*! Ensures the current storage block has enough space for the given
        number of characters.  If the remaining space in the current storage
        block is not enough, a new storage block is allocated.

        \param charsToUse Number of characters.
    */
    void ensureBlockCapacity(int charsToAlloc);

    /*! Determines if a string needing the given storage space should be
        allocated in its own block.  The default implementation returns true
        if charsToAlloc is at least half the size of a storage block.

        \param charsToAlloc Number of characters.
        \return Flag indicating whether a separate block should be allocated.
    */
    virtual bool shouldAllocAlone(int charsToAlloc);

    /*! Inserts the given character string into this string block.  This will
        copy the string into its own block if shouldAllocAlone returns true.

        \param str Character string to be copied.
        \return Pointer to character string as stored in this string block.
    */
    char* insertString(const char* str, int charsToAlloc);


public:

    /*! Adds the given character string to this string block.
        
        \param str Character string to be copied.
        \return Pointer to character string as stored in this string block.
    */
    char* addString(const char* str);

    /*! Adds the given character string to this string block, providing enough
        space for the string to grow to some known maximum length.

        \param str Character string to be copied.
        \return Pointer to character string as stored in this string block.
    */
    char* addString(const char* str, int maxLength);

    /*! Adds the given string to this string block.
        
        \param str String to be copied.
        \return Pointer to character string as stored in this string block.
        \throws R3CERR_ILLEGALARGUMENT If str is NULL.
    */
    char* addString(R3CString* str);

    /*! Adds the given string to this string block, providing enough space for
        the string to grow to some known maximum length.

        \param str Character string to be copied.
        \return Pointer to character string as stored in this string block.
        \throws R3CERR_ILLEGALARGUMENT If str is NULL.
    */
    char* addString(R3CString* str, int maxLength);


}; // end R3CStringBlock


/* R3CStringBlockStack */

// Class definition with doxygen comments

/*! Provides a large, expandable stack of memory for storage of groups of
 *  character strings.  Calls to \ref addString add the string to the current
 *  stack level.  Calling \ref push increases the stack level.  Calling \ref
 *  pop frees up all memory on the current stack level, then decreases the
 *  stack level.
 */
class R3CStringBlockStack
: public R3CStringBlock {

// Member Variables

protected:

    //! The current stack level, where the starting index is 0.
    int stackLevel;

    //! Array of pointers to the start of each stack level.
    char** stackStartPtr;

    //! Number of stack pointers currently allocated.
    int stackStartPtrAlloc;
    

// Construction

private:

    //! Initializes this string block stack.
    void init();

public:

    //! Creates a new string block storage stack.
    R3CStringBlockStack();

    /*! Creates a new string block storage stack.  Each new storage block will
        be allocated of the given size.
        
        \param kbPerBlock Number of kilobytes per storage block.
    */
    R3CStringBlockStack(int kbPerBlock);


// Destruction

public:

    //! Destructor.
    ~R3CStringBlockStack();


// Manage String Stack

public:

    /*! Retrieves the current stack level.
        
        \return Current stack level, where the first stack level is 0.
    */
    int getStackLevel();
    
    /*! Pushes the stack to a new level.
        
        \return New stack level.
    */
    int push();

    /*! Pops off the current stack level.
        
        \return New stack level.
    */
    int pop();


}; // end R3CStringBlockStack


/* R3CUnicode */

// Class definition with doxygen comments

/*! Stores a dynamically allocated unicode character string.
 *  Unicode string objects will expand in size as necessary.  Note that
 *  subsequent calls to \ref getChars are not guaranteed to return the same
 *  pointer.
 *
 *  The current implementation allocates storage in multiples of 128.  If a
 *  string is expected to grow larger than 128, it is good practice to specify
 *  the expected capacity during construction.
 */
class R3CUnicode {

// Member Variables

protected:

    //! Unicode character string.
    wchar_t* str;

    //! Current length of string, not including the zero-terminator.
    int curLength;

    //! Current maximum length of string, not including the zero-terminator.
    int maxLength;


// Construction

private:

    //! Set all member variables to their initial values.
    void initPrefill();

    /*! Initializes the storage capacity.
        
        \param capacity New storage capacity.
    */
    void initMaxLength(int capacity);

public:

    //! Creates a new empty unicode string.
    R3CUnicode();

    /*! Creates a new empty unicode string, with the given storage capacity.
        
        \param capacity Storage capacity.
        \throws R3CERR_ILLEGALARGUMENT If capacity is less than 1.
    */
    R3CUnicode(int capacity);

    /*! Creates a new unicode string, copied from the source character string.
        If the source character string is NULL, the new unicode string will be
        empty.
        
        \param sourceStr Source character string.
    */
    R3CUnicode(const wchar_t* sourceStr);

    /*! Creates a new unicode string, copied from the source character string,
        with the given storage capacity.  If the source character string is a
        NULL, the new string will be empty.

        \param sourceStr Source character string.
        \param capacity Storage capacity.
        \throws R3CERR_ILLEGALARGUMENT If capacity is less than the length of
            the source character string, or less than 1.
    */
    R3CUnicode(const wchar_t* sourceStr, int capacity);

    /*! Creates a new unicode string, copied from the given unicode string.

        \param strToCopy Unicode string to be copied.
        \throws R3CERR_ILLEGALARGUMENT If a NULL pointer is passed.
    */
    R3CUnicode(R3CString* strToCopy);


// Destruction

public:

    //! Destructor.
    ~R3CUnicode();


// Retrieve String Information

public:

    /*! Returns the underlying character string.
        This method is available with the understanding that calling code
        should not perform write operations that affect the length of the
        string.  It is preferred that calling code use the returned character
        string for read-only purposes.
        
        \return Character string.
    */
    wchar_t* getChars();

    /*! Returns the length of this string, not including the zero-terminator.
        
        \return Length of this string.
    */
    int getLength();

    /*! Returns the current storage capacity.
        
        \return Current storage capacity.
    */
    int getCapacity();


// Conversion To / From ASCII

public:

    /*! Converts this string into a standard ASCII string.
        
        \param targetStr Target string.
        \return Length of the string.
    */
    int toAscii(R3CString* result);

    /*! Replaces this string with the source ASCII character string.
        
        \param sourceStr Source ASCII character string.
        \return Length of the string.
    */
    int fromAscii(const char* strToCopy);

    /*! Replaces this string with the source ASCII string.
        
        \param sourceStr Source ASCII string.
        \return Length of the string.
    */
    int fromAscii(R3CString* sourceStr);


// Find Sub-strings

public:

    /*! Returns the character at the given character position.

        \param pos Character position, where the first character is at
            position 0.
        \return Unicode character.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
    */
    wchar_t getCharAt(int pos);

    /*! Finds the first occurrence of the given character.
        
        \param charToFind Character to find.
        \return Position of the given character, or -1 if the character was
            not found.
    */
    int find(wchar_t charToFind);

    /*! Finds the first occurrence of the given sub-string.
        
        \param strToFind Sub-string to find.
        \return Position of the given sub-string, or -1 if the sub-string was
            not found.
    */
    int find(const wchar_t* strToFind);

    /*! Finds the last occurrence of the given character.

        \param charToFind Character to find.
        \return Position of the given character, or -1 if the character was
            not found.
    */
    int findReverse(wchar_t charToFind);


// Comparing Strings

public:

    /*! Compares this string to the given character string.

        \param str Character string to compare to.
        \return Value that is less than 0, equal to 0, or greater than 0; as
            the passed string is less than, equal to, or greater than this
            string.
        \throws R3CERR_ILLEGALARGUMENT If str is NULL.
    */
    int compare(const wchar_t* str);

    /*! Compares this string to the given string.

        \param str String to compare to.
        \return Value that is less than 0, equal to 0, or greater than 0; as
            the passed string is less than, equal to, or greater than this
            string.
        \throws R3CERR_ILLEGALARGUMENT If str is NULL.
    */
    int compare(R3CUnicode* str);


// Update String

public:

    /*! Ensures the storage capacity can handle a string of the given length.
        This is called internally whenever an operation expands the length of
        the string.  It is provided as a public method in the rare case that
        calling code needs to modify the length of the string externally.

        \param newLength String length.
    */
    void ensureCapacity(int newLength);

    /*! Resets the length of the string, based on the actual character string
        stored.  This is provided as a public method in the rare case that
        calling code needs to modify the length of the string externally.
        
        \return Length of the string.
    */
    int resetLength();

    /*! Replaces this string with the source character string.  If the source
        character string is NULL, the string is cleared.
        
        \param sourceStr Source character string.
        \return Length of the string.
    */
    int set(const wchar_t* sourceStr);

    /*! Replaces this string with the source string.
        
        \param sourceStr Source string.
        \return Length of the string.
        \throws R3CERR_ILLEGALARGUMENT If sourceStr is NULL.
    */
    int set(R3CUnicode* sourceStr);

    /*! Appends the given character to the end of this string.

        \param charToAppend Character to append.
        \return Number of characters appended (always 1).
        \throws R3CERR_ILLEGALARGUMENT If charToAppend is a null-terminator.
    */
    int append(wchar_t charToAppend);

    /*! Appends the source character string to the end of this string.  If the
        source character string is NULL, 0 is returned.

        \param sourceStr Source string.
        \return Number of characters appended.
    */
    int append(const wchar_t* sourceStr);

    /*! Appends charCount characters, starting at startPos, from sourceStr
        into this string.  If sourceStr is NULL, 0 is returned.  If startPos
        is greater than the length of sourceStr, no characters are appended.
        Only characters up to the end of sourceStr are appended.

        \param sourceStr Source character string.
        \param startPos Starting character position.
        \param charCount Number of characters to append.
        \return Number of characters actually appended.
        \throws R3CERR_ILLEGALARGUMENT If startPos is less than 0.
    */
    int append(const wchar_t* sourceStr, int startPos, int charCount);

    /*! Appends the source string to the end of this string.
        
        \param sourceStr Source string.
        \return Number of characters appended.
        \throws R3CERR_ILLEGALARGUMENT If sourceStr is NULL.
    */
    int append(R3CString* sourceStr);

    /*! Inserts the given character at the given position in this string.
        
        \param pos Position to insert into this string.
        \param charToInsert Character to insert.
        \return Number of characters inserted (always 1).
        \throws R3CERR_OUTOFRANGE If pos less than 0, or greater or equal to
            the length of this string.
    */
    int insert(int pos, wchar_t charToInsert);

    /*! Inserts the source character string at the given position in this
        string.

        \param pos Position to insert into this string.
        \param sourceStr Source character string.
        \return Number of characters inserted.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
    */
    int insert(int pos, wchar_t* strToInsert);

    /*! Inserts charCount characters, starting at startPos, from sourceStr
        into this string at pos.  If sourceStr is NULL, 0 is returned.  If
        startPos is greater than the length of sourceStr, no characters are
        inserted.  Only characters up to the end of sourceStr are appended.

        \param pos Position to insert into this string.
        \param sourceStr Source character string.
        \param startPos Starting character position.
        \param charCount Number of characters to insert.
        \return Number of characters actually inserted.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
        \throws R3CERR_ILLEGALARGUMENT If startPos is less than 0.
    */
    int insert(int pos, wchar_t* strToInsert, int startPos, int charCount);

    /*! Inserts the source string into this string.
        
        \param pos Position to insert into this string.
        \param sourceStr Source string.
        \return Number of characters actually inserted.
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
    */
    int insert(int pos, R3CUnicode* strToInsert);

    /*! Removes the character at the given position from this string.
        
        \param pos Position of character in this string.
        \return Number of characters deleted (always 1).
        \throws R3CERR_OUTOFRANGE If pos is less than 0, or greater or equal
            to the length of this string.
    */
    int deleteCharAt(int pos);

    /*! Removes all characters in this string between startPos and endPos,
        inclusive.

        \param startPos Starting position in this string.
        \param endPos Ending position in this string.
        \return Number of characters deleted.
        \throws R3CERR_OUTOFRANGE If startPos less than 0, or endPos is
            greater or equal to the length of this string.
    */
    int deleteChars(int startPos, int endPos);

    /*! Clears all characters from this string.
    */
    void clear();


}; // end R3CUnicode


#endif
