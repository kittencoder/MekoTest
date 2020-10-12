/**
 * Global definitions used throughout the Meko code
 */
#ifndef MEKO_DEFINITIONS_H
#define MEKO_DEFINITIONS_H

// ### for a more descriptive code
// for use in pure-virtual-methods
#define PURE 0
#define MUST_BE_IMPLEMENTED 0

// ### Return values
// main-method status-returns
#define RETURN_OK 0
#define RETURN_ERROR_LOADING 1  // error during loadup-phase
#define RETURN_FAIL_SEE_LOG 2   // check the last log entry, it should explain a lot...

// ### Support for development, supress warnings in development when they are not required to be fixed asap and other warnings/errors are more important
#ifdef DEBUG
// snippets against non-important compiler warnings
#define NOT_USED_FOR_NOW(x) ((void)(x))
#else
#define NOT_USED_FOR_NOW(x)
#endif
/**
 * if your possible children might require this argument
 * or if you don't require this argument from the parent
 * you can use this
 */
#define NOT_USED(x) ((void)(x))

// ### Simple Template Constructs
#ifdef SA
#error SA already defined
#endif
#define SA(...) __VA_ARGS__

// ### math defines
#define PRECISE_PI 3.141592653589793238462643383279502884L

#endif