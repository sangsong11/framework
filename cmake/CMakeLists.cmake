cmake_minimum_required(VERSION 2.8)
project(bmc-setip)

#指定C++标准的版本
set(CMAKE_CXX_STANDARD 14)

#进行交叉编译器配置
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CMAKE_C_COMPILER /opt/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/arrch64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER /opt/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/arrch64-linux-gnu-g++)
SET(CMAKE_C_FLAGS -Wno -unused-result)

#定义第三方库路径变量
set(THIRD_PARTY_RELATIVE_PATH ../3rd_party/linux/ft)
set(THIRD_PARTY_ABSOLUTE_PATH  $(PROJECT_SOURCE_DIR)/src/3rd_party/linux/ft)

IF(CMAKE_BUILD_TYPE MATHCHES Debug)
     ADD_DEFINITONS(-Dm32 -g -O0 -DARP_DECLARE_STATIC
	                -DARP_HAS_DSO
					-DAPU_DECLARE_STATIC
					-D_GNU_SOURCE
					-D_DEBUG)
ELSEIF (CMAKE_BUILD_TYPE MATHCHES Release)
      ADD_DEFINITONS(-Dm32  -DARP_DECLARE_STATIC
	                -DARP_HAS_DSO
					-DAPU_DECLARE_STATIC
					-D_GNU_SOURCE)
ELSE()
     MESSAGE (STATUS "No DEFINITIONS")
ENDIF()
	 
IF(BOARD_TYPE MATCHES system)
    ADD_DEFINITONS(-DRUNTARGET=1)
ELSEIF(BOARD_TYPE MATCHES control)
    ADD_DEFINITONS(-DRUNTARGET=0)
ELSE()
    MESSAGE (STATUS "No DEFINITIONS")
ENDIF()

add_subdirectory(./src/liblog)
add_subdirectory(./src/libosal)
add_subdirectory(./src/libipconf)
add_subdirectory(./src/cli)