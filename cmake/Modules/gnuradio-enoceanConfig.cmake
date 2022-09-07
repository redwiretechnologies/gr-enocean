find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_ENOCEAN gnuradio-enocean)

FIND_PATH(
    GR_ENOCEAN_INCLUDE_DIRS
    NAMES gnuradio/enocean/api.h
    HINTS $ENV{ENOCEAN_DIR}/include
        ${PC_ENOCEAN_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_ENOCEAN_LIBRARIES
    NAMES gnuradio-enocean
    HINTS $ENV{ENOCEAN_DIR}/lib
        ${PC_ENOCEAN_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-enoceanTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_ENOCEAN DEFAULT_MSG GR_ENOCEAN_LIBRARIES GR_ENOCEAN_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_ENOCEAN_LIBRARIES GR_ENOCEAN_INCLUDE_DIRS)
