find_library(LIB_RD_KAFKA rdkafka)
IF(LIB_RD_KAFKA)
    MESSAGE(STATUS "RdKafka libraries founded: OK ${LIB_RD_KAFKA}")
ENDIF(LIB_RD_KAFKA)

IF(NOT LIB_RD_KAFKA)
    MESSAGE(FATAL_ERROR "ERROR: unable to link RdKafka")
ENDIF(NOT LIB_RD_KAFKA)