INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BB60C bb60c)

FIND_PATH(
    BB60C_INCLUDE_DIRS
    NAMES bb60c/api.h
    HINTS $ENV{BB60C_DIR}/include
        ${PC_BB60C_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BB60C_LIBRARIES
    NAMES gnuradio-bb60c
    HINTS $ENV{BB60C_DIR}/lib
        ${PC_BB60C_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BB60C DEFAULT_MSG BB60C_LIBRARIES BB60C_INCLUDE_DIRS)
MARK_AS_ADVANCED(BB60C_LIBRARIES BB60C_INCLUDE_DIRS)

