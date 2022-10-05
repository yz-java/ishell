SET(Libssh2_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${Libssh2_PATH}
)

FIND_PATH(Libssh2_INCLUDE_DIR 
    NAMES libssh2.h
    HINTS
    $ENV{Libssh2DIR}
    PATH_SUFFIXES include
    PATHS ${Libssh2_SEARCH_PATHS}
)

FIND_LIBRARY(Libssh2_LIBRARY
 NAMES ssh2
 PATH_SUFFIXES lib
 PATHS ${Libssh2_SEARCH_PATHS}  
 REQUIRED
)

if(Libssh2_LIBRARY)
	get_filename_component(Libssh2_LIBRARY_DIR ${Libssh2_LIBRARY} DIRECTORY)
endif()

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Libssh2 REQUIRED_VARS Libssh2_LIBRARY Libssh2_LIBRARY_DIR Libssh2_INCLUDE_DIR)
