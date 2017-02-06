# - Find SoundTouch library
# This module defines
#  SOUNDTOUCH_INCLUDE_DIR
#  SOUNDTOUCH_LIBRARIES
#  SOUNDTOUCH_FOUND
#
# vim: expandtab sw=4 ts=4 sts=4:

include(FindPkgConfig)
pkg_check_modules (SOUNDTOUCH_PKG QUIET soundtouch)

find_path(SOUNDTOUCH_INCLUDE_DIR NAMES soundtouch/SoundTouch.h
  PATHS
  ${SOUNDTOUCH_PKG_INCLUDE_DIRS}
  /usr/include/soundtouch
  /usr/include
  /usr/local/include/soundtouch
  /usr/local/include
)

find_library(SOUNDTOUCH_LIBRARIES NAMES soundtouch SoundTouch
  PATHS
  ${SOUNDTOUCH_PKG_LIBRARY_DIRS}
  /usr/lib
  /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SOUNDTOUCH
                                  REQUIRED_VARS SOUNDTOUCH_LIBRARIES SOUNDTOUCH_INCLUDE_DIR)

if(SOUNDTOUCH_FOUND)
  if(NOT TARGET SoundTouch::SoundTouch)
    add_library(SoundTouch::SoundTouch UNKNOWN IMPORTED)
    set_target_properties(SoundTouch::SoundTouch PROPERTIES
      IMPORTED_LOCATION ${SOUNDTOUCH_LIBRARIES}
      INTERFACE_INCLUDE_DIRECTORIES ${SOUNDTOUCH_INCLUDE_DIR}
    )
  endif()
endif()

mark_as_advanced(SOUNDTOUCH_INCLUDE_DIR SOUNDTOUCH_LIBRARIES)
