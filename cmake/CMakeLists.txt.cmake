cmake_minimum_required(VERSION 2.8)

include_directories(../../../comm)
include_directories(../libosal)
include_directories(../libipconf/base)
include_directories($(THIRD_PARTY_RELATIVE_PATH)/jasson/include)

aux_source_directory(./base SRC_LIST)
aux_source_directory(./session SRC_LIST)
aux_source_directory(./util SRC_LIST)
aux_source_directory(./rpc SRC_LIST)

message($(SRC_LIST))
ADD_LIBRARY(bmc STATIC $(SRC_LIST))
