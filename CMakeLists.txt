cmake_minimum_required(VERSION 3.8)
project(bmc VERSION 0.1.0)
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_C_STANDARD 99)

#Common And code

include(DarwinCheck)

find_library(LIB_PAHO paho-mqtt3c)
IF(LIB_PAHO)
    MESSAGE(STATUS "MQTT libraries founded: OK - ${LIB_PAHO}")
ENDIF(LIB_PAHO)

IF(NOT LIB_PAHO)
    MESSAGE(FATAL_ERROR "ERROR: unable to link MQTT")
ENDIF(NOT LIB_PAHO)

find_library(LIB_FFTW fftw3)
IF(LIB_FFTW)
    MESSAGE(STATUS "FFTW libraries founded: OK ${LIB_FFTW}")
ENDIF(LIB_FFTW)

IF(NOT LIB_FFTW)
    MESSAGE(FATAL_ERROR "ERROR: unable to link FFTW")
ENDIF(NOT LIB_FFTW)

set(BASIC_SOURCE_FILES process-basic/process-basic.c process-basic/process-basic.h)
set(COMMON_SOURCE_FILES ${BASIC_SOURCE_FILES} common/cmdparse.c common/common.c common/data_reader.c common/data_reader.h common/types.c common/types.h common/packet_reader.c common/packet_reader.h common/client.h)
set(MQTT_SOURCE_FILES mqtt/mqtt.c)
add_definitions(-DVERSION=\"${PROJECT_VERSION}\")

set(COMMON_LIBS fftw3 m)

#Executables
add_executable(bmc_full ${COMMON_SOURCE_FILES} ${MQTT_SOURCE_FILES} full.c)
set_target_properties(bmc_full PROPERTIES COMPILE_DEFINITIONS FULL=1)
TARGET_LINK_LIBRARIES(bmc_full paho-mqtt3c ${LIBS_SYSTEM} ${COMMON_LIBS})

add_executable(bmc_mqtt ${COMMON_SOURCE_FILES} ${MQTT_SOURCE_FILES} mqtt/mqttmain.c process-basic/process-basic.c process-basic/process-basic.h)
target_link_libraries(bmc_mqtt paho-mqtt3c ${LIBS_SYSTEM} ${COMMON_LIBS})