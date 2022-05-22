cmake_minimum_required(VERSION 3.9.0)

add_definitions(-DANDROID_HELIUM)
set(build_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSK_GL -DSK_BUILD_FOR_ANDROID -DFOLLY_NO_CONFIG=1 -DFOLLY_HAVE_CLOCK_GETTIME=1 -DFOLLY_HAVE_MEMRCHR=1 -DFOLLY_USE_LIBCPP=1 -DFOLLY_MOBILE=1 -DON_ANDROID -DONANDROID")

set (SKIA_DIR "../skialib")
set (PACKAGE_NAME "helium")
set (ANDROID_HELIUM_INC_DIR "cpp")
file(GLOB RENDER_DIR "../cpp/render/*.cpp" "../cpp/rener/*.h")
file(GLOB RENDER_INC "../cpp/render")
file(GLOB JSI_INC "../cpp/jsi")
file(GLOB JSI_SRC "${JSI_INC}/*.cpp" "${JSI_INC}/*.h")
file(GLOB ANDROID_SRC "cpp/*.cpp" "cpp/*.h")

file(GLOB libfbjni_link_DIRS "${build_DIR}/fbjni*.aar/jni/${ANDROID_ABI}")
file(GLOB libfbjni_include_DIRS "${build_DIR}/fbjni-*-headers.jar/")
file (GLOB LIBRN_DIR "${build_DIR}/react-native-0*/jni/${ANDROID_ABI}")
message(STATUS "here i am!!!!!!!!!!! ${LIBRN_DIR}")

find_library(SKIA_LIB skia PATHS "${SKIA_DIR}/arm64" NO_DEFAULT_PATH)

find_library( # Defines the name of the path variable that stores the
        # location of the NDK library.
        log-lib

        # Specifies the name of the NDK library that
        # CMake needs to locate.
        log )

find_library( # Sets the name of the path variable.
        android-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        android )


#link_directories("../cpp/lib/skia/out/arm64")


find_library(
        JSI_LIB
        jsi
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
        REQUIRED
)

find_library(
        FBJNI_LIBRARY
        fbjni
        PATHS ${libfbjni_link_DIRS}
        NO_CMAKE_FIND_ROOT_PATH
        REQUIRED
)

find_library(
        TURBOMODULES_LIB
        turbomodulejsijni
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
        REQUIRED
)


add_library(${PACKAGE_NAME}
        SHARED
        ${ANDROID_SRC}
        ${RENDER_DIR}
        ${JSI_SRC}
        )

include_directories(
        ${NODE_MODULES_DIR}/react-native/React
        ${NODE_MODULES_DIR}/react-native/React/Base
        ${NODE_MODULES_DIR}/react-native/ReactCommon/callinvoker
        ${NODE_MODULES_DIR}/react-native/ReactCommon/jsi
        ${NODE_MODULES_DIR}/react-native/ReactCommon/react/nativemodule/core
        ${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/java/com/facebook/react/turbomodule/core/jni
        ${libfbjni_include_DIRS}
        ${ANDROID_HELIUM_INC_DIR}
        ${RENDER_INC}
        ${JSI_INC}
        ${SKIA_DIR}
        cpp/
)

set_target_properties(
        ${PACKAGE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(
        ${PACKAGE_NAME}
        ${FBJNI_LIBRARY}
        ${JSI_LIB}
        ${TURBOMODULES_LIB}
        ${SKIA_LIB}
        -ljnigraphics
        -lGLESv2
        -lEGL
        ${log-lib}
        ${android-lib}
)
