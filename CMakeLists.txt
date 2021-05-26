cmake_minimum_required(VERSION 3.5)

project(ishell LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(
  "./"
  "Libs/openssl/include"
  "Libs/ssh2/include"
)

LINK_DIRECTORIES(
  "Libs/openssl/lib"
  "Libs/ssh2/lib"
)
set(target_link_libraries_flag "crypto" "ssl" "ssh2")
IF (WIN32)
  MESSAGE(STATUS "Now is windows")
  set(executable_flag "WIN32" "app_win32.rc")
  set(target_link_libraries_flag "ws2_32" "libcrypto" "libssl" "libssh2")
  MESSAGE(STATUS "target_link_libraries_flag ==> ${target_link_libraries_flag}")
  
  # set(QT_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5")
  # set(Qt5_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5")
  # set(Qt5Core_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Core")
  # set(Qt5Gui_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Gui")
  # set(Qt5Network_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Network")
  # set(Qt5Positioning_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Positioning")
  # set(Qt5PrintSupport_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5PrintSupport")
  # set(Qt5Qml_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Qml")
  # set(Qt5Quick_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Quick")
  # set(Qt5WebChannel_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5WebChannel")
  # set(Qt5WebEngineCore_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5WebEngineCore")
  # set(Qt5WebEngineWidgets_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5WebEngineWidgets")
  # set(Qt5Widgets_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Widgets")
  # set(Qt5Sql_DIR "C:/Qt/Qt5.12.10/5.12.10/msvc2017_64/lib/cmake/Qt5Sql")
ELSEIF (APPLE)
  MESSAGE(STATUS "Now is Apple systens.")
    
ELSEIF (UNIX)
	MESSAGE(STATUS "Now is UNIX-like OS's.")
ENDIF ()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Concurrent Sql WebSockets WebChannel WebEngineWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Concurrent Sql WebSockets WebChannel WebEngineWidgets REQUIRED)


FILE(GLOB SRC_LIST_CPP *.cpp)

file(GLOB_RECURSE QRC_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.qrc)

file(GLOB UI_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.ui)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/components SRC_LIST_CPP)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/db SRC_LIST_CPP)

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
  ${target_link_libraries_flag}
)


