
/****************************************************************************
 * Riley's C++ Commons Library -- I/O
 * Copyright (C) 2010, Ryan Robinson
 *
 * Ryan Robinson
 * Web Site:  http://www.riley-man.com/
 * E-mail: riley@riley-man.com
 ****************************************************************************/

/*! \file r3c-io.hpp
 *  
 *  This is the include file for retrieving or persisting data with streams.
 */

#ifndef _r3_commons_io_HPP_
#define _r3_commons_io_HPP_


// *** ADDITIONAL INCLUDES *** //

#include "r3c-string.hpp"

#include <stdio.h>
#include <exception>


// *** DECLARATIONS *** //

// Exceptions

//! Exception indicating a stream being opened was already open.
extern const char* R3CERR_IO_STREAMALREADYOPEN;

//! Exception indicating a stream has not yet been opened.
extern const char* R3CERR_IO_STREAMNOTOPEN;

//! Exception indicating a stream was not found.
extern const char* R3CERR_IO_STREAMNOTFOUND;

//! Exception indicating that an unrecoverable I/O error has occurred.
extern const char* R3CERR_IO_EXCEPTION;


// Interface List

class R3CStream;
class R3CTextInputStream;
class R3CTextOutputStream;
class R3CBinaryInputStream;
class R3CBinaryOutputStream;
class R3CRandomAccessStream;


// Class List

class R3CTextInputFile;
class R3CTextInputMemBlock;
class R3CTextOutputFile;
class R3CTextOutputMemBlock;


// *** INTERFACE DEFINITIONS *** //

/* R3CStream */

// Class definition with doxygen comments

//! Represents any byte stream.
class R3CStream {

// Destruction

public:

    //! Destructor.
    virtual ~R3CStream() = 0;

// Close Stream

public:

    /*! Closes the stream.
        
        \throws R3CERR_IO_STREAMNOTOPEN If the stream was not open.
        \throws R3CERR_IO_EXCEPTION If an I/O error occurred.
    */
    virtual void close() = 0;

}; // end R3CStream


/* R3CTextInputStream */

// Class definition with doxygen comments

//! Represents a character input stream.
class R3CTextInputStream {

// Destruction

public:

    //! Destructor.
    virtual ~R3CTextInputStream() = 0;

public:

    /*! Returns a flag indicating whether the input stream has characters
        ready to be read.

        \return Flag indicating whether the input stream has characters ready
            to be read.
        \throws R3CERR_IO_STREAMNOTOPEN If the stream was not open.
    */
    virtual bool ready() = 0;
    
    /*! Reads the next available character.  This method will block until some
        input is available, an I/O error occurs, or the end of the stream is
        reached.
        
        \return The character read, or EOF if the end of the stream has been
            reached.
        \throws R3CERR_IO_STREAMNOTOPEN If the stream was not open.
        \throws R3CERR_IO_EXCEPTION If an I/O error occurred.
    */
    virtual int readChar() = 0;

    /*! Reads numChars characters into the target string.  This method will
        block until some input is available, an I/O error occurs, or the end
        of the stream is reached.

        \param targetStr Target string to receive the input characters.
        \param numChars Number of characters to read.
        \return Number of characters actually read, or EOF if the end of the
            stream has been reached.
        \throws R3CERR_ILLEGALARGUMENT If targetStr is NULL, or numChars is
            less than 0.
        \throws R3CERR_IO_STREAMNOTOPEN If the stream was not open.
        \throws R3CERR_IO_EXCEPTION If an I/O error occurred.
    */
    virtual int readChars(R3CString* targetStr, int numChars) = 0;

    /*! Reads the next non-blank line into the target string.  This method
        will block until some input is available, an I/O error occurs, or the
        end of the stream is reached.

        \param targetStr Target string to receive the input characters.
        \return Number of characters read, or EOF if the end of the stream has
            been reached.
        \throws R3CERR_ILLEGALARGUMENT If targetStr is NULL.
        \throws R3CERR_IO_STREAMNOTOPEN If the stream was not open.
        \throws R3CERR_IO_EXCEPTION If an I/O error occurred.
    */
    virtual int readLine(R3CString* targetStr) = 0;

}; // end R3CTextInputStream


// *** CLASS DEFINITIONS *** //

/* R3CTextInputFile */

// Class definition with doxygen comments

//! Represents a text file being used as an input stream.
class R3CTextInputFile :
    public R3CStream,
    public R3CTextInputStream
{

// Member Variables

private:

    //! File handle.
    FILE* fileHandle;


// Construction

public:

    //! Creates a new input stream from an unspecified text file.
    R3CTextInputFile();

    /*! Creates and open a new input stream from the given text file.
        
        \param inputFilename Path to file.
        \throws R3CERR_ILLEGALARGUMENT If inputFilename is NULL.
        \throws R3CERR_IO_STREAMALREADYOPEN If the stream was already open.
        \throws R3CERR_IO_STREAMNOTFOUND If the file could not be opened.
    */
    R3CTextInputFile(const char* inputFilename);

    /*! Creates and opens a new input stream from the given text file.
        
        \param inputFilename Path to file.
        \throws R3CERR_ILLEGALARGUMENT If inputFilename is NULL.
        \throws R3CERR_IO_STREAMALREADYOPEN If the stream was already open.
        \throws R3CERR_IO_STREAMNOTFOUND If the file could not be opened.
    */
    R3CTextInputFile(R3CString* inputFilename);


// Destruction

public:

    //! Destructor.
    ~R3CTextInputFile();


// Open Stream

public:

    /*! Opens the given input file.
        
        \param inputFilename Path to file.
        \throws R3CERR_ILLEGALARGUMENT If inputFilename is NULL.
        \throws R3CERR_IO_STREAMALREADYOPEN If the stream was already open.
        \throws R3CERR_IO_STREAMNOTFOUND If the file could not be opened.
    */
    void open(const char* inputFilename);

    /*! Opens the given input file.
        
        \param inputFilename Path to file.
        \throws R3CERR_ILLEGALARGUMENT If inputFilename is NULL.
        \throws R3CERR_IO_STREAMALREADYOPEN If the stream was already open.
        \throws R3CERR_IO_STREAMNOTFOUND If the file could not be opened.
    */
    void open(R3CString* inputFilename);


// Check For Stream Readiness

public:

    bool ready();


// Read Characters from the File

public:

    int readChar();

    int readChars(R3CString* targetStr, int numChars);

    int readLine(R3CString* targetStr);


// Close File

public:

    /*! Closes the input file.

        \throws R3CERR_IO_STREAMNOTOPEN If the stream was not open.
        \throws R3CERR_IO_EXCEPTION If an I/O error occurred.
    */
    void close();


}; // end R3CTextInputFile


#endif
