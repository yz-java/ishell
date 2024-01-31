cmake_minimum_required(VERSION 3.5)
if(WIN32)
  set(CMAKE_TOOLCHAIN_FILE D:/vcpkg/scripts/buildsystems/vcpkg.cmake)
elseif(APPLE)
  set(CMAKE_TOOLCHAIN_FILE /Users/yangzhao/Desktop/my_project/vcpkg/scripts/buildsystems/vcpkg.cmake)
elseif(UNIX)
  set(CMAKE_TOOLCHAIN_FILE /home/yangzhao/project/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif(WIN32)
project(ishell)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

option(ENABLE_WS "use websocket" ON)
option(ENABLE_NOVNC_CLIENT "use noVnc client" ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Concurrent Sql WebChannel WebEngineWidgets WebSockets OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Concurrent Sql WebChannel WebEngineWidgets WebSockets OpenGL REQUIRED)

IF (WIN32)
  MESSAGE(STATUS "Now is windows")
  set(executable_flag "WIN32" "app_win32.rc")
  list(APPEND target_link_libraries_flags "ws2_32")
ELSEIF (APPLE)
  MESSAGE(STATUS "Now is Apple systens.")
ELSEIF (UNIX)
  MESSAGE(STATUS "Now is UNIX-like OS's.")
  set(target_link_libraries_flags )
  list(APPEND target_link_libraries_flags "pthread" "X11" "-lXext" "-lXcursor" "rt" "freerdp" "winpr" "freerdp-client" "SDL2::SDL2")
ENDIF ()

find_package(OpenSSL REQUIRED)
find_package(SDL2 REQUIRED)
message(STATUS "SDL2_INCLUDE_DIR ==> ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES ==> ${SDL2_LIBRARIES}")
include_directories(${SDL2_INCLUDE_DIRS})


find_package(Libssh2 CONFIG REQUIRED)
# include_directories(${Libssh2_INCLUDE_DIR})
# link_directories(${Libssh2_LIBRARY_DIR})

find_package(LibVNCServer CONFIG REQUIRED)

FILE(GLOB SRC_LIST_CPP *.c *.cpp *.h components/*.h)

file(GLOB_RECURSE QRC_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.qrc)

file(GLOB UI_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.ui components/*.ui)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/components SRC_LIST_CPP)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/db SRC_LIST_CPP)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/utils SRC_LIST_CPP)

find_package(OpenGL)
if(OpenGL_FOUND)
    message(STATUS "OpenGL_FOUND")
    set(ENABLE_NOVNC_CLIENT OFF)
    list(APPEND target_link_libraries_flags "LibVNCServer::vncclient")
    if(APPLE)
        list(APPEND target_link_libraries_flags "-framework OpenGL")
    elseif(UNIX)
        list(APPEND target_link_libraries_flags "GL")
    endif()
else()
    list(REMOVE_ITEM SRC_LIST_CPP "${CMAKE_CURRENT_SOURCE_DIR}/vncviewerwidget.h" "${CMAKE_CURRENT_SOURCE_DIR}/vncviewerwidget.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/vncclient.h" "${CMAKE_CURRENT_SOURCE_DIR}/vncclient.cpp")
endif()

message(STATUS "target_link_libraries_flags ==> ${target_link_libraries_flags}")
if(APPLE)
elseif(UNIX)
  find_package(FreeRDP CONFIG REQUIRED)
  message(STATUS "FreeRDP_INCLUDE_DIR is ${FreeRDP_INCLUDE_DIR}")
  find_package(WinPR CONFIG REQUIRED)
  message(STATUS "WinPR_INCLUDE_DIR is ${WinPR_INCLUDE_DIR}")
  find_package(FreeRDP-Client CONFIG REQUIRED)
  list(APPEND SRC_LIST_CPP xfree-rdp/xf_client.h xfree-rdp/xfreerdp.h)
  aux_source_directory(${PROJECT_SOURCE_DIR}/xfree-rdp SRC_LIST_CPP)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
    # SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
    # SET(CMAKE_INSTALL_RPATH "\${ORIGIN}/lib")
endif()

ADD_EXECUTABLE(
  ${PROJECT_NAME} ${executable_flag}
  ${SRC_LIST_CPP}
  ${UI_SOURCE_FILES}
  ${QRC_SOURCE_FILES}
)

if(ENABLE_WS)
  message(STATUS "ws is enabled")
  target_compile_definitions(${PROJECT_NAME} PRIVATE  ENABLE_WS)
  # add_definitions(-DENABLE_WS)
endif()
if(ENABLE_NOVNC_CLIENT)
    target_compile_definitions(${PROJECT_NAME} PRIVATE  USE_NOVNC_CLIENT)
endif()
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE  WIN32_LEAN_AND_MEAN)
endif()
if(FreeRDP_FOUND)
message(STATUS "support xfreerdp")
target_compile_definitions(${PROJECT_NAME} PRIVATE  XFREE_RDP)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_RPATH "$ORIGIN/lib")

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
  Qt${QT_VERSION_MAJOR}::OpenGL
  $<IF:$<TARGET_EXISTS:Libssh2::libssh2_shared>,Libssh2::libssh2_shared,Libssh2::libssh2_static>
  OpenSSL::SSL OpenSSL::Crypto
  ${target_link_libraries_flags}
)


