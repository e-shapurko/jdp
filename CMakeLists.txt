cmake_minimum_required(VERSION 3.12)
project(jdp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{QT_TOOLS_PATH})
    set(QT_TOOLS_PATH $ENV{QT_TOOLS_PATH})
else() 
    set(QT_TOOLS_PATH "D:/Qt/Tools/mingw1310_64")
endif()

set(MINGW_DIR "${QT_TOOLS_PATH}/bin")

set(CMAKE_C_COMPILER "${MINGW_DIR}/gcc.exe")
set(CMAKE_CXX_COMPILER "${MINGW_DIR}/g++.exe")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include") 
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(SOURCES
    "${SRC_DIR}/jtable.cpp"
    "${SRC_DIR}/jobject.cpp"
    "${SRC_DIR}/jserializer.cpp"
    "${SRC_DIR}/jnode/jobjectnode.cpp"
    "${SRC_DIR}/jnode/jtablenode.cpp"
)

set(HEADERS
    "${INCLUDE_DIR}/jnode/ijbasic.h"
    "${INCLUDE_DIR}/jnode/ijobject.h"
    "${INCLUDE_DIR}/jnode/ijnode.h"
    "${INCLUDE_DIR}/jnode/jobjectnode.h"
    "${INCLUDE_DIR}/jnode/jintnode.h"
    "${INCLUDE_DIR}/jnode/jfloatnode.h"
    "${INCLUDE_DIR}/jnode/jstringnode.h"
    "${INCLUDE_DIR}/jnode/jboolnode.h"
    "${INCLUDE_DIR}/jnode/jtablenode.h"    
    "${INCLUDE_DIR}/jtypes.h"
    "${INCLUDE_DIR}/jtable.h"
    "${INCLUDE_DIR}/jobject.h"
    "${INCLUDE_DIR}/jserializer.h"
)

add_library(jdp SHARED ${SOURCES} ${HEADERS})

target_compile_features(jdp PRIVATE cxx_std_20)

target_include_directories(jdp PRIVATE 
    ${INCLUDE_DIR}
    ${INCLUDE_DIR}/jnode
    ${INCLUDE_DIR}/boost
)

if(MINGW)
    target_link_options(jdp PRIVATE -static -lwsock32)
endif()

install(TARGETS jdp
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)