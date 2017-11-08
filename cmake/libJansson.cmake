find_library(LIB_JSON jansson)
IF(LIB_JSON)
    MESSAGE(STATUS "JSon libraries founded: OK ${LIB_JSON}")
ENDIF(LIB_JSON)

IF(NOT LIB_JSON)
    MESSAGE(FATAL_ERROR "ERROR: unable to link JSon")
ENDIF(NOT LIB_JSON)