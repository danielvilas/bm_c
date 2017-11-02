cmake_minimum_required(VERSION 3.8)
project(bmc VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)

#Common And code
find_library(LIB_PAHO paho-mqtt3c)
IF(LIB_PAHO)
    MESSAGE(STATUS "MQTT libraries founded: OK")
ENDIF(LIB_PAHO)

IF(NOT LIB_PAHO)
    MESSAGE(FATAL_ERROR "ERROR: unable to link MQTT")
ENDIF(NOT LIB_PAHO)

set(COMMON_SOURCE_FILES common/cmdparse.c common/common.c)
set(MQTT_SOURCE_FILES mqtt/mqtt.c)
add_definitions(-DVERSION=\"${PROJECT_VERSION}\")

#Executables
add_executable(bmc_full ${COMMON_SOURCE_FILES} ${MQTT_SOURCE_FILES} full.c)
set_target_properties(bmc_full PROPERTIES COMPILE_DEFINITIONS FULL=1)
TARGET_LINK_LIBRARIES(bmc_full paho-mqtt3c)

add_executable(bmc_mqtt ${COMMON_SOURCE_FILES} ${MQTT_SOURCE_FILES} mqtt/mqttmain.c)
target_link_libraries(bmc_mqtt paho-mqtt3c)