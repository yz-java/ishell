#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LibVNCServer::vncclient" for configuration ""
set_property(TARGET LibVNCServer::vncclient APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(LibVNCServer::vncclient PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libvncclient.so.0.9.13"
  IMPORTED_SONAME_NOCONFIG "libvncclient.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS LibVNCServer::vncclient )
list(APPEND _IMPORT_CHECK_FILES_FOR_LibVNCServer::vncclient "${_IMPORT_PREFIX}/lib/libvncclient.so.0.9.13" )

# Import target "LibVNCServer::vncserver" for configuration ""
set_property(TARGET LibVNCServer::vncserver APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(LibVNCServer::vncserver PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libvncserver.so.0.9.13"
  IMPORTED_SONAME_NOCONFIG "libvncserver.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS LibVNCServer::vncserver )
list(APPEND _IMPORT_CHECK_FILES_FOR_LibVNCServer::vncserver "${_IMPORT_PREFIX}/lib/libvncserver.so.0.9.13" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
