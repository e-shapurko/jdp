if(DEFINED ENV{QT_TOOLS_PATH})
    set(QT_TOOLS_PATH $ENV{QT_TOOLS_PATH})
else() 
    set(QT_TOOLS_PATH "D:/Qt/Tools/mingw1310_64")
endif()    

if(DEFINED ENV{CMAKE_PATH})
    set(CMAKE_PATH $ENV{CMAKE_PATH})
else() 
    set(CMAKE_PATH "C:/Program Files/CMake/bin")
endif()

set(QT_TOOLS_BIN_PATH "${QT_TOOLS_PATH}/bin")

if(WIN32)
    set(MAKER "${CMAKE_PATH}/cmake.exe")
    set(QT_BUILDER "${QT_TOOLS_BIN_PATH}/mingw32-make.exe")
endif()

set(SCRIPT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(BUILD_DIR "${SCRIPT_DIR}/build")

if(NOT EXISTS "${BUILD_DIR}")
    file(REMOVE_RECURSE "${BUILD_DIR}")
endif()    
file(MAKE_DIRECTORY "${BUILD_DIR}")

get_filename_component(PROJECT_PATH "${SCRIPT_DIR}/.." ABSOLUTE)

set(BOOST_LIB "${PROJECT_PATH}/boost")
set(JDP_INC_DIR "${PROJECT_PATH}/jdp/include")

if(EXISTS "${BOOST_LIB}")
    file(COPY "${BOOST_LIB}" DESTINATION "${JDP_INC_DIR}/")
else()
    message(FATAL_ERROR "Папка ${BOOST_LIB} не существует")
endif()

set(ENV{PATH} "${QT_TOOLS_BIN_PATH};${CMAKE_PATH}")

execute_process(
    COMMAND "${MAKER}" -G "MinGW Makefiles" ..
    WORKING_DIRECTORY "${BUILD_DIR}"
    RESULT_VARIABLE cmake_result
)

if(NOT cmake_result EQUAL 0)
    message(FATAL_ERROR "qmake failed")
endif()

execute_process(
    COMMAND "${QT_BUILDER}"
    WORKING_DIRECTORY "${BUILD_DIR}"
    RESULT_VARIABLE make_result
)

if(NOT make_result EQUAL 0)
    message(FATAL_ERROR "make failed")
endif()

message("Build completed")