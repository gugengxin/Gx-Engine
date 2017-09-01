#!/bin/sh
set -e

cd $(dirname $0)
PROJECT_ROOT=$PWD
cd ../..
LIBRARY_ROOT=$PWD
cd ../..
GX_ROOT=$PWD
cd $PROJECT_ROOT

source $GX_ROOT/bin/cmake.sh
source $GX_ROOT/bin/xcode.sh
source $GX_ROOT/bin/ninja.sh

SRC_ROOT=${LIBRARY_ROOT}/src

function copyInclude {
	OUTPUT_H_DIR="$1"
	CMAKE_CACHE_DIR="$2"
	if [ -d $OUTPUT_H_DIR ]; then
		rm -rf $OUTPUT_H_DIR
	fi
	cp -r ${SRC_ROOT}/include $OUTPUT_H_DIR
	cp -f ${CMAKE_CACHE_DIR}/include/freetype/config/ftconfig.h $OUTPUT_H_DIR/freetype/config/ftconfig.h
	cp -f ${CMAKE_CACHE_DIR}/include/freetype/config/ftoption.h $OUTPUT_H_DIR/freetype/config/ftoption.h
}

#iOS
mkdir -p build/ios/device
cd build/ios/device
cmake -GNinja -D"CMAKE_TOOLCHAIN_FILE=${GX_ROOT}/bin/ios.toolchain.cmake" -DIOS_PLATFORM="OS" -D"FREETYPE_NO_DIST=true" -D"WITH_ZLIB=OFF" -D"WITH_BZip2=OFF" -D"WITH_PNG=OFF" -D"WITH_HarfBuzz=OFF" ${SRC_ROOT}
ninja -C .
cd $PROJECT_ROOT

mkdir -p build/ios/simulator
cd build/ios/simulator
cmake -GNinja -D"CMAKE_TOOLCHAIN_FILE=${GX_ROOT}/bin/ios.toolchain.cmake" -DIOS_PLATFORM="SIMULATOR" -D"FREETYPE_NO_DIST=true" -D"WITH_ZLIB=OFF" -D"WITH_BZip2=OFF" -D"WITH_PNG=OFF" -D"WITH_HarfBuzz=OFF" ${SRC_ROOT}
ninja -C .
cd $PROJECT_ROOT

mkdir -p build/ios/simulator64
cd build/ios/simulator64
cmake -GNinja -D"CMAKE_TOOLCHAIN_FILE=${GX_ROOT}/bin/ios.toolchain.cmake" -DIOS_PLATFORM="SIMULATOR64" -D"FREETYPE_NO_DIST=true" -D"WITH_ZLIB=OFF" -D"WITH_BZip2=OFF" -D"WITH_PNG=OFF" -D"WITH_HarfBuzz=OFF" ${SRC_ROOT}
ninja -C .
cd $PROJECT_ROOT

OUTPUT_DIR=$LIBRARY_ROOT/lib/ios
if [ -d $OUTPUT_DIR ] 
then
rm -rf $OUTPUT_DIR
fi
mkdir -p $OUTPUT_DIR
lipo -create build/ios/device/libfreetype.a build/ios/simulator/libfreetype.a build/ios/simulator64/libfreetype.a -output $OUTPUT_DIR/libfreetype.a

copyInclude $LIBRARY_ROOT/include/ios $PROJECT_ROOT/build/ios/device

#TVOS
mkdir -p build/atv/device
cd build/atv/device
cmake -GNinja -D"CMAKE_TOOLCHAIN_FILE=${GX_ROOT}/bin/appletv.toolchain.cmake" -DTVOS_PLATFORM="OS" -D"FREETYPE_NO_DIST=true" -D"WITH_ZLIB=OFF" -D"WITH_BZip2=OFF" -D"WITH_PNG=OFF" -D"WITH_HarfBuzz=OFF" ${SRC_ROOT}
ninja -C .
cd $PROJECT_ROOT

mkdir -p build/atv/simulator
cd build/atv/simulator
cmake -GNinja -D"CMAKE_TOOLCHAIN_FILE=${GX_ROOT}/bin/appletv.toolchain.cmake" -DTVOS_PLATFORM="SIMULATOR" -D"FREETYPE_NO_DIST=true" -D"WITH_ZLIB=OFF" -D"WITH_BZip2=OFF" -D"WITH_PNG=OFF" -D"WITH_HarfBuzz=OFF" ${SRC_ROOT}
ninja -C .
cd $PROJECT_ROOT

OUTPUT_DIR=$LIBRARY_ROOT/lib/atv
if [ -d $OUTPUT_DIR ] 
then
rm -rf $OUTPUT_DIR
fi
mkdir -p $OUTPUT_DIR
lipo -create build/atv/device/libfreetype.a build/atv/simulator/libfreetype.a -output $OUTPUT_DIR/libfreetype.a

copyInclude $LIBRARY_ROOT/include/atv $PROJECT_ROOT/build/atv/device

#MacOSX
mkdir -p build/mac
cd build/mac
cmake -GNinja -D"CMAKE_TOOLCHAIN_FILE=${GX_ROOT}/bin/mac.toolchain.cmake" -D"FREETYPE_NO_DIST=true" -D"WITH_ZLIB=OFF" -D"WITH_BZip2=OFF" -D"WITH_PNG=OFF" -D"WITH_HarfBuzz=OFF" ${SRC_ROOT}
ninja -C .
cd $PROJECT_ROOT

OUTPUT_DIR=$LIBRARY_ROOT/lib/mac
if [ -d $OUTPUT_DIR ] 
then
rm -rf $OUTPUT_DIR
fi
mkdir -p $OUTPUT_DIR
lipo -create build/mac/libfreetype.a -output $OUTPUT_DIR/libfreetype.a

copyInclude $LIBRARY_ROOT/include/mac $PROJECT_ROOT/build/mac

rm -rf build
