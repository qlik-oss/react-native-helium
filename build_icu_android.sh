#!/bin/bash
export TARGET=aarch64-linux-android
export TOOLCHAIN=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/darwin-x86_64
export API=29
export ANDROIDVER=$API
export NDK_STANDARD_ROOT=/Users/vel/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib/aarch64-linux-android/29

export AR=$TOOLCHAIN/bin/llvm-ar
export CC=$TOOLCHAIN/bin/$TARGET$API-clang
export AS=$CC
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
export STRIP=$TOOLCHAIN/bin/llvm-strip
export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
export LD=$TOOLCHAIN/bin/ld
export LDFLAGS="-lc -lstdc++ -stdlib=libc++ -L$NDK_STANDARD_ROOT -Wl,--gc-sections,-rpath-link=$NDK_STANDARD_ROOT"


export CXXFLAGS="-Os -fvisibility=hidden -std=c11 -std=c++17 -fno-short-wchar -fdata-sections -ffunction-sections -DU_STATIC_IMPLEMENTATION -DU_USING_ICU_NAMESPACE=0 -DU_DISABLE_RENAMING=1 -DU_HAVE_NL_LANGINFO_CODESET=0 -DU_TIMEZONE=0 -DANDROID"
# export CFLAGS="-nostdlib -nostartfiles -nodefaultlibs -ffreestanding"
export CFLAGS="-fvisibility=hidden -nostartfiles -ffreestanding -Os -std=c17 -fno-short-wchar -fdata-sections -ffunction-sections -DU_STATIC_IMPLEMENTATION -DU_USING_ICU_NAMESPACE=0 -DU_DISABLE_RENAMING=1 -DU_HAVE_NL_LANGINFO_CODESET=0 -DU_TIMEZONE=0 -DANDROID"

echo $TOOLCHAIN
echo $TARGET

configureArgs="--enable-icuio=no --enable-static=yes --enable-shared=no --enable-samples=no --enable-tests=no --enable-tools=no --enable-extras=no --with-data-packaging=static --enable-renaming=no"
echo $configureArgs


(exec "../source/configure" --with-cross-build="/Users/vel/workspace/icu/icu4c/macOS" \
    $configureArgs --host=$TARGET --prefix="$PWD/icu_build")

make --verbose
make install