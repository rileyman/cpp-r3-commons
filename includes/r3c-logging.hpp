
// *** CLASS DEFINITIONS *** //

// Class R3CDebugFunctionState

class R3CDebugFunctionState {

// Member Variables

protected:

    //! String stack being used for this function.
    R3CStringBlock* threadStack;
    
    //! Pointer to class name, or NULL if none.
    char* className;

    //! Pointer to class instance, or NULL if none.
    void* classObj;

    //! Identifying text for class instance, or NULL if none.
    char* classObjIdText;

    //! Pointer to function name, or NULL if none.
    char* funcName;

    //! Number of parameters passed into this function.
    int paramCount;

    //! Array of parameter name pointers passed into this function.
    char** paramNames;

    //! Array of parameter data types passed into this function.
    // TODO: decide on whether we even want this...

    //! Array of parameter value pointers passed into this function.
    char** paramValues;

    //! Stack of step strings, indicating where processing within this function
    //! has reached.
    R3CStringBlock* stepStack;

    //! Number of class instance member variables tracked.
    int memberVarCount;

    //! Array of member variable name pointers tracked.
    char** memberVarNames;

    //! Array of member variable value pointers tracked.
    char** memberVarValues;

};

