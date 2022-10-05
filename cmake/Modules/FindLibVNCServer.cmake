SET(LibVNCServer_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${LibVNCServer_PATH}
)

FIND_PATH(LibVNCServer_INCLUDE_DIR rfb.h
    HINTS
    $ENV{LibVNCServerDIR}
    PATH_SUFFIXES include/rfb include
    PATHS ${LibVNCServer_SEARCH_PATHS}
)

if(LibVNCServer_INCLUDE_DIR)
	get_filename_component(LibVNCServer_INCLUDE_DIR ${LibVNCServer_INCLUDE_DIR} DIRECTORY)
endif()

set(LibVNCServer_LIBRARY_NAMES "vncclient" "vncserver")
FIND_LIBRARY(LibVNCServer_LIBRARYS
 NAMES ${LibVNCServer_LIBRARY_NAMES}
 PATH_SUFFIXES lib
 PATHS ${LibVNCServer_SEARCH_PATHS}  
 REQUIRED
)
if(LibVNCServer_LIBRARYS)
	get_filename_component(LibVNCServer_LIBRARY_DIR ${LibVNCServer_LIBRARYS} DIRECTORY)
endif()
INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibVNCServer REQUIRED_VARS LibVNCServer_LIBRARYS LibVNCServer_LIBRARY_DIR LibVNCServer_INCLUDE_DIR)
