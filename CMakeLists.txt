cmake_minimum_required(VERSION 3.1.0)

project(nbteditor VERSION 1.2)
set (NBTEDITOR_VERSION 1.2)

find_package(Qt5Widgets)

find_package( ZLIB REQUIRED )
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
endif(ZLIB_FOUND)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/src/Version.h")

set(nbteditor_res "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc")
# For Windows release build: set(nbteditor_res "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc" "${CMAKE_CURRENT_SOURCE_DIR}/resources/nbteditor.rc" "${CMAKE_CURRENT_SOURCE_DIR}/resources/qt_de.qm")
file(GLOB_RECURSE nbteditor_src "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/UI/*.ui")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/window")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/resources")
include_directories( ${CMAKE_BINARY_DIR} )

## set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

# Tell CMake to create the nbteditor executable
add_executable(nbteditor ${MOC_HEADERS} ${nbteditor_src} ${nbteditor_res})
# For Windows release build: add_executable(nbteditor WIN32 ${MOC_HEADERS} ${nbteditor_src} ${nbteditor_res})

# Use the Widgets module from Qt 5.
target_link_libraries(nbteditor Qt5::Widgets ${ZLIB_LIBRARIES})
