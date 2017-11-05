find_library(LIB_PAHO paho-mqtt3c)
IF(LIB_PAHO)
    MESSAGE(STATUS "MQTT libraries founded: OK - ${LIB_PAHO}")
ENDIF(LIB_PAHO)

IF(NOT LIB_PAHO)
    MESSAGE(FATAL_ERROR "ERROR: unable to link MQTT")
ENDIF(NOT LIB_PAHO)