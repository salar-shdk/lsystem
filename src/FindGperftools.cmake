# Find Gperftools
# This module defines
#   GPERFTOOLS_INCLUDE_DIR, where to find gperftools headers
#   GPERFTOOLS_LIBRARIES, the libraries to link against

find_path(GPERFTOOLS_INCLUDE_DIR google/profiler.h)

find_library(GPERFTOOLS_PROFILER_LIB profiler)

set(GPERFTOOLS_LIBRARIES ${GPERFTOOLS_PROFILER_LIB})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gperftools DEFAULT_MSG
    GPERFTOOLS_INCLUDE_DIR GPERFTOOLS_LIBRARIES)

mark_as_advanced(GPERFTOOLS_INCLUDE_DIR GPERFTOOLS_LIBRARIES)

