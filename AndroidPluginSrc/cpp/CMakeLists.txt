# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)
project("opennativec")
SET(APP_CPPFLAGS "${APP_CPPFLAGS}  -fexceptions -v")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/mapper_cv/)
include_directories(${PROJECT_SOURCE_DIR}/plugin/)
include_directories(${PROJECT_SOURCE_DIR}/plugin/unity/)

#opencv
set(OpenCV_STATIC on)
set(OpenCV_DIR C:/ProjectStudy/libraries/opencv4.5.3-vc14-15/opencv-4.5.3-android-sdk/OpenCV-android-sdk/sdk/native/jni)
find_package(OpenCV REQUIRED)

add_library(native-cv SHARED
        mapper_cv/opencv_reader.cpp
        )

add_library(
        NativeAndroidPlugin
        SHARED
        RenderingPlugin.cpp
        plugin/RenderAPI.cpp
        plugin/RenderAPI_OpenGLCoreES.cpp
)

add_library(native-lib # Sets the name of the library.
        SHARED native-lib.cpp)

find_library(log-lib log)
find_library(native-media-lib mediandk)
find_library(android-lib android)
find_library(jnigraphics-lib jnigraphics)
find_library(gl-lib GLESv2)

target_link_libraries(
        native-cv
        ${OpenCV_LIBS}
        ${jnigraphics-lib}
        ${log-lib}
)
target_link_libraries(
        native-lib # Specifies the target library..
        native-cv
        ${log-lib})

target_link_libraries(
        NativeAndroidPlugin
        native-cv
        ${gl-lib}
        ${log-lib})
