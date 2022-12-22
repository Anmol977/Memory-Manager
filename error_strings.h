//
// Created by anmol on 3/12/22.
//

#ifndef MEMALLOCATOR_ERROR_STRINGS_H
#define MEMALLOCATOR_ERROR_STRINGS_H

#include <string>

namespace error_strings {
    static const char *INSUFFICIENT_MEMORY = "Operation failed. Ran out of memory...";
    static const char *INVALID_ALLOCATION_PARAMETER = "Invalid allocation parameter provided.";
    static const char *INITIALIZATION_ERROR = "Error initializing the package.";
    static const char *NO_HEAP_EXTEND = "Failed to extend heap.";
}

namespace info_strings {
    static const char *NO_COALESCE_EVENT = "No Coalescing occured !";
    static const char *COALESCING_OCCURED = "YAY!";
}


#endif //MEMALLOCATOR_ERROR_STRINGS_H
