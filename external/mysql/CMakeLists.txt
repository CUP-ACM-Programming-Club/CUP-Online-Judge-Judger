cmake_minimum_required(VERSION 3.9)
project(judge_mysql)
set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
if (EXISTS "/usr/local/bin/gcc-9")
    set(CMAKE_C_COMPILER /usr/local/bin/gcc-9)
    set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9)
elseif (EXISTS "/usr/local/bin/gcc-8")
    set(CMAKE_C_COMPILER /usr/local/bin/gcc-8)
    set(CMAKE_CXX_COMPILER /usr/local/bin/g++-8)
elseif (EXISTS "/usr/local/bin/gcc")
    set(CMAKE_C_COMPILER /usr/local/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/local/bin/g++)
else ()
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif ()
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_path(MYSQL_INCLUDE_DIR
        NAMES "mysql.h"
        PATHS "/usr/include/mysql"
        "/usr/local/include/mysql"
        "/usr/mysql/include/mysql")

find_library(MYSQL_LIBRARY
        NAMES "mysqlclient" "mysqlclient_r"
        PATHS "/lib/mysql"
        "/lib64/mysql"
        "/usr/lib/mysql"
        "/usr/lib64/mysql"
        "/usr/local/lib/mysql"
        "/usr/local/lib64/mysql"
        "/usr/mysql/lib/mysql"
        "/usr/mysql/lib64/mysql")


if (MYSQL_INCLUDE_DIR AND EXISTS "${MYSQL_INCLUDE_DIRS}/mysql_version.h")
    file(STRINGS "${MYSQL_INCLUDE_DIRS}/mysql_version.h"
            MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$")
    string(REGEX REPLACE
            "^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
            "${MYSQL_VERSION_H}")
endif ()

# handle the QUIETLY and REQUIRED arguments and set MYSQL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MYSQL DEFAULT_MSG
        REQUIRED_VARS MYSQL_LIBRARY MYSQL_INCLUDE_DIR
        VERSION_VAR MYSQL_VERSION_STRING)

set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARY)
include_directories(${MYSQL_INCLUDE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/out)
add_library(mysql SHARED MySQLSubmissionAdapter.cpp MySQLSubmissionAdapter.h MySQLSubmissionInfoManager.cpp MySQLSubmissionInfoManager.h MySQLAutoPointer.cpp MySQLAutoPointer.h)
target_link_libraries(mysql ${MYSQL_LIBRARIES})