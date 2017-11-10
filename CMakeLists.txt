cmake_minimum_required(VERSION 3.6)
project(bmc VERSION 0.1.0)
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_C_STANDARD 99)

#Common And code

include(DarwinCheck)
include(FixUsrLocal)

include(libPaho)
include(libFftw)
include(libFann)
include(libRdKafka)
include(libZookeeper)
include(libJansson)
include(libCurl)

set(BASIC_SOURCE_FILES process-basic/process-basic.c process-basic/process-basic.h)
set(COMMON_SOURCE_FILES ${BASIC_SOURCE_FILES} common/cmdparse.c common/common.c common/data_reader.c common/data_reader.h common/types.c common/types.h common/packet_reader.c common/packet_reader.h common/client.h)
set(MQTT_SOURCE_FILES mqtt/mqtt.c mqtt/mqtt.h)
set(KAFKA_SOURCE_FILES protocol-kafka/kafka.c protocol-kafka/kafka.h)
set(REST_SOURCE_FILES protocol-rest/rest.c protocol-rest/rest.h)
set(SOAP_SOURCE_FILES protocol-soap/soap.c protocol-soap/soap.h)

add_definitions(-DVERSION=\"${PROJECT_VERSION}\")


set(COMMON_LIBS fftw3 m doublefann)
set(KAFKA_LIBS zookeeper_mt rdkafka jansson )

#Executables
add_executable(bmc_full ${COMMON_SOURCE_FILES} ${MQTT_SOURCE_FILES} ${KAFKA_SOURCE_FILES} ${REST_SOURCE_FILES} ${SOAP_SOURCE_FILES} full.c)
set_target_properties(bmc_full PROPERTIES COMPILE_DEFINITIONS FULL=1)
TARGET_LINK_LIBRARIES(bmc_full ${KAFKA_LIBS} paho-mqtt3c curl ${LIBS_SYSTEM} ${COMMON_LIBS})

add_executable(bmc_mqtt ${COMMON_SOURCE_FILES} ${MQTT_SOURCE_FILES} mqtt/mqttmain.c)
target_link_libraries(bmc_mqtt paho-mqtt3c ${LIBS_SYSTEM} ${COMMON_LIBS})

add_executable(bmc_kafka ${COMMON_SOURCE_FILES} ${KAFKA_SOURCE_FILES} protocol-kafka/kafkamain.c)
target_link_libraries(bmc_kafka ${KAFKA_LIBS} ${LIBS_SYSTEM} ${COMMON_LIBS})

add_executable(bmc_rest ${COMMON_SOURCE_FILES} ${REST_SOURCE_FILES} protocol-rest/restmain.c)
target_link_libraries(bmc_rest curl ${LIBS_SYSTEM} ${COMMON_LIBS})

add_executable(bmc_soap ${COMMON_SOURCE_FILES} ${SOAP_SOURCE_FILES} protocol-soap/soapmain.c)
target_link_libraries(bmc_soap curl ${LIBS_SYSTEM} ${COMMON_LIBS})