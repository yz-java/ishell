cmake_minimum_required(VERSION 3.5)

project(ishell)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Concurrent Sql WebSockets WebChannel WebEngineWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Concurrent Sql WebSockets WebChannel WebEngineWidgets REQUIRED)

set(target_link_libraries_flag "ssh2" "pthread" "X11" "-lXext" "-lXcursor" "rt")
IF (WIN32)
  MESSAGE(STATUS "Now is windows")

  set(executable_flag "WIN32" "app_win32.rc")
  set(target_link_libraries_flag "ws2_32" "libssh2" "zlib")
  set(LibVNCServer_PATH "")
  set(Libssh2_PATH "")

#   set(QT_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5")
#   set(Qt5_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5")
#   set(Qt5Core_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Core")
#   set(Qt5Gui_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Gui")
#   set(Qt5Network_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Network")
#   set(Qt5Positioning_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Positioning")
#   set(Qt5PrintSupport_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5PrintSupport")
#   set(Qt5Qml_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Qml")
#   set(Qt5Quick_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Quick")
#   set(Qt5WebChannel_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5WebChannel")
#   set(Qt5WebEngineCore_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5WebEngineCore")
#   set(Qt5WebEngineWidgets_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5WebEngineWidgets")
#   set(Qt5Widgets_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Widgets")
#   set(Qt5Sql_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Sql")
ELSEIF (APPLE)
  MESSAGE(STATUS "Now is Apple systens.")

ELSEIF (UNIX)
  MESSAGE(STATUS "Now is UNIX-like OS's.")
  add_definitions(-DUNIX)
  set(LibVNCServer_PATH "/home/yangzhao/project/libvncserver/Release")
  set(Libssh2_PATH "/home/yangzhao/project/libssh2/Release")
  set(FreeRDP_PATH "/home/yangzhao/project/FreeRDP/Release")
ENDIF ()

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(Libssh2 REQUIRED)
include_directories(${Libssh2_INCLUDE_DIR})
link_directories(${Libssh2_LIBRARY_DIR})

find_package(LibVNCServer REQUIRED)
include_directories(${LibVNCServer_INCLUDE_DIR})
link_directories(${LibVNCServer_LIBRARY_DIR})

find_package(FreeRDP REQUIRED)
include_directories(${FreeRDP_INCLUDE_DIR} ${WINPR_INCLUDE_DIR})
link_directories(${FreeRDP_LIBRARY_DIR})
message(STATUS "FreeRDP_LIBRARY_DIR ${FreeRDP_LIBRARY_DIR}")
FILE(GLOB SRC_LIST_CPP *.c *.cpp *.h)

file(GLOB_RECURSE QRC_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.qrc)

file(GLOB UI_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.ui)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/components SRC_LIST_CPP)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/db SRC_LIST_CPP)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/utils SRC_LIST_CPP)
if(UNIX)
list(APPEND SRC_LIST_CPP xfreerdp/xf_client.h xfreerdp/xfreerdp.h)
aux_source_directory(${PROJECT_SOURCE_DIR}/xfreerdp SRC_LIST_CPP)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

ADD_EXECUTABLE(
  ${PROJECT_NAME} ${executable_flag}
  ${SRC_LIST_CPP}
  ${UI_SOURCE_FILES}
  ${QRC_SOURCE_FILES}
)

target_link_libraries( 
  ${PROJECT_NAME} PRIVATE  
  Qt${QT_VERSION_MAJOR}::Widgets 
  Qt${QT_VERSION_MAJOR}::Core  
  Qt${QT_VERSION_MAJOR}::Gui 
  Qt${QT_VERSION_MAJOR}::WebChannel 
  Qt${QT_VERSION_MAJOR}::WebEngineWidgets  
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Concurrent
  Qt${QT_VERSION_MAJOR}::WebSockets
  vncclient
  SDL2
  freerdp3
  freerdp-client3
  winpr3
  ${OPENSSL_LIBRARIES}
  ${target_link_libraries_flag}
)


