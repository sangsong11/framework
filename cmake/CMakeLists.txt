cmake_minimum_required(VERSION 2.8)


include_directories(../../../comm)
include_directories(../libosal)
include_directories(../libipconf/base)
include_directories($(THIRD_PARTY_RELATIVE_PATH)/jasson/include)
include_directories($(THIRD_PARTY_RELATIVE_PATH)/libevent/include)

link_directories($(THIRD_PARTY_RELATIVE_PATH)/jansson/lib)
link_directories($(THIRD_PARTY_RELATIVE_PATH)/libevent/lib)

set(SRC_LIST main.c)
add_executable(bmc-setip $(SRC_LIST))

target_link_libraries(bmc-setip bmc)
target_link_libraries(bmc-setip log)
target_link_libraries(bmc-setip osal)

MESSAGE(STATUS "this is BINARY " $(PROJECT_BINARY_DIR))
MESSAGE(STATUS "this is BINARY " $(PROJECT_SOURCE_DIR))

set(CMAKE_INSTALL_PREFIX $(PROJECT_SOURCE_DIR)/CLion_output)
install(TARGETS bmc-setip RUNTIME DESTINATION $(CMAKE_INSTALL_PREFIX))
