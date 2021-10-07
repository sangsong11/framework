#!/bin/bash

echo "*****************************************************************************************"
echo "*****************************start build bmc-setip***************************************"

if [ -d ./build ]; then
  rm -rf ./build
fi

mkdir build
cd build
board_type=" "
if [ $# -ne 2 ];then
    echo "the parameter is not equal to 2  example ./a.out release system"
	exit 1
fi

if [ $1 = "release" ] && [ $2 = "system" ];then
    board_type="system"
	cmake -DCMAKE_BUILD_TYPE=Release -DBOARD_TYPE=system ../
elif [ $1 = "release" ] && [ $2 = "control" ];then
    board_type="control"
	cmake -DCMAKE_BUILD_TYPE=Release -DBOARD_TYPE=control ../
elif [ $1 = "debug" ] && [ $2 = "system" ];then
    board_type="system"
	cmake -DCMAKE_BUILD_TYPE=Debug -DBOARD_TYPE=system ../
elif [ $1 = "debug" ] && [ $2 = "control" ];then
    board_type="control"
	cmake -DCMAKE_BUILD_TYPE=Debug -DBOARD_TYPE=control ../
else
    echo "parameter is not correct \n"
	exit 1
fi

make clean
make 

cd ../../

if [ -d ./output ];then
   rm -rf ./output
fi

mkdir output
cd output
mkdir bmc-setip

cd ../
if [ $board_type = "system" ];then
   echo "--------------------------------------------------------$board_type"
   cd bmc_setip/build/src/clibmc-setip output/bmc-setip/bmc-setip-system
   cd output/bmc-setip
   md5sum bmc-setip-system >> bmc-setip-system.md5
fi

if [ $board_type = "control" ];then
   echo "--------------------------------------------------------$board_type"
   cd bmc_setip/build/src/clibmc-setip output/bmc-setip/bmc-setip-control
   cd output/bmc-setip
   md5sum bmc-setip-control >> bmc-setip-control.md5
fi

echo "*****************************************************************************************"
echo "*****************************end build bmc-setip***************************************"
