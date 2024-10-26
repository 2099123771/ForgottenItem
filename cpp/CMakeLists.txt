cmake_minimum_required(VERSION 3.22.1)

project("ywzw")

set(CMAKE_CXX_STANDARD 20)



set(PUBLIC_INCLUDE_DIRS
        BNM-Android-master/include
        BNM-Android-master/external/include
)

set(INCLUDE_DIRS
        BNM-Android-master/include
        BNM-Android-master/external/include
        BNM-Android-master/external
        BNM-Android-master/external/utf8
        BNM-Android-master/src/private
)

add_library(
        BNM
        STATIC
        BNM-Android-master/src/Class.cpp
        BNM-Android-master/src/ClassesManagement.cpp
        BNM-Android-master/src/Coroutine.cpp
        BNM-Android-master/src/Defaults.cpp
        BNM-Android-master/src/Delegates.cpp
        BNM-Android-master/src/EventBase.cpp
        BNM-Android-master/src/Exceptions.cpp
        BNM-Android-master/src/FieldBase.cpp
        BNM-Android-master/src/Hooks.cpp
        BNM-Android-master/src/Image.cpp
        BNM-Android-master/src/Internals.cpp
        BNM-Android-master/src/Loading.cpp
        BNM-Android-master/src/MethodBase.cpp
        BNM-Android-master/src/MonoStructures.cpp
        BNM-Android-master/src/PropertyBase.cpp
        BNM-Android-master/src/UnityStructures.cpp
        BNM-Android-master/src/Utils.cpp
)

target_include_directories(
        BNM
        PUBLIC
        ${INCLUDE_DIRS}
)

set_property(TARGET BNM PROPERTY BNM_INCLUDE_DIRECTORIES ${PUBLIC_INCLUDE_DIRS})


add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        BNM
        log)