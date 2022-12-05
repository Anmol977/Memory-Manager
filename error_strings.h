//
// Created by anmol on 3/12/22.
//

#ifndef MEMALLOCATOR_ERROR_STRINGS_H
#define MEMALLOCATOR_ERROR_STRINGS_H

#include <string>

namespace error_strings {
    static const char *INSUFFICIENT_MEMORY = "Operation failed. Ran out of memory...";
    static const char *INVALID_ALLOCATION_PARAMETER = "Invalid allocation parameter provided.";
}

namespace info_strings {
    static const char *NO_COALESCE_EVENT = "No Coalescing occured !";
}


#endif //MEMALLOCATOR_ERROR_STRINGS_H
