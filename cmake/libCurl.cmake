find_library(LIB_CURL curl)
IF(LIB_CURL)
    MESSAGE(STATUS "Curl libraries founded: OK ${LIB_CURL}")
ENDIF(LIB_CURL)

IF(NOT LIB_CURL)
    MESSAGE(FATAL_ERROR "ERROR: unable to link Curl")
ENDIF(NOT LIB_CURL)