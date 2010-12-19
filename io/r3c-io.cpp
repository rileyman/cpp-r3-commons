
#include "../includes/r3c-io.hpp"


// *** EXCEPTIONS *** //

const char* R3CERR_IO_STREAMALREADYOPEN = "R3CERR_IO_STREAMALREADYOPEN";

const char* R3CERR_IO_STREAMNOTOPEN = "R3CERR_IO_STREAMNOTOPEN";

const char* R3CERR_IO_STREAMNOTFOUND = "R3CERR_IO_STREAMNOTFOUND";

const char* R3CERR_IO_EXCEPTION = "R3CERR_IO_EXCEPTION";


// *** INTERFACES *** //

R3CStream::~R3CStream() {
}

R3CTextInputStream::~R3CTextInputStream() {
}
