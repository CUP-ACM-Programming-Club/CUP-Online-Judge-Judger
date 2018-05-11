cmake_minimum_required(VERSION 3.9)
project(judge_client)
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_C_COMPILER /usr/local/bin/gcc)
# set(CMAKE_CXX_COMPILER /usr/local/bin/g++)
# - Try to find MySQL.
# Once done this will define:
# MYSQL_FOUND			- If false, do not try to use MySQL.
# MYSQL_INCLUDE_DIRS	- Where to find mysql.h, etc.
# MYSQL_LIBRARIES		- The libraries to link against.
# MYSQL_VERSION_STRING	- Version in a string of MySQL.
#
# Created by RenatoUtsch based on eAthena implementation.
#
# Please note that this module only supports Windows and Linux officially, but
# should work on all UNIX-like operational systems too.
#

#=============================================================================
# Copyright 2012 RenatoUtsch
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)


    find_path( MYSQL_INCLUDE_DIR
            NAMES "mysql.h"
            PATHS "/usr/include/mysql"
            "/usr/local/include/mysql"
            "/usr/mysql/include/mysql" )

    find_library( MYSQL_LIBRARY
            NAMES "mysqlclient" "mysqlclient_r"
            PATHS "/lib/mysql"
            "/lib64/mysql"
            "/usr/lib/mysql"
            "/usr/lib64/mysql"
            "/usr/local/lib/mysql"
            "/usr/local/lib64/mysql"
            "/usr/mysql/lib/mysql"
            "/usr/mysql/lib64/mysql" )


if( MYSQL_INCLUDE_DIR AND EXISTS "${MYSQL_INCLUDE_DIRS}/mysql_version.h" )
    file( STRINGS "${MYSQL_INCLUDE_DIRS}/mysql_version.h"
            MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$" )
    string( REGEX REPLACE
            "^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
            "${MYSQL_VERSION_H}" )
endif()

# handle the QUIETLY and REQUIRED arguments and set MYSQL_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( MYSQL DEFAULT_MSG
        REQUIRED_VARS	MYSQL_LIBRARY MYSQL_INCLUDE_DIR
        VERSION_VAR		MYSQL_VERSION_STRING )

set( MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} )
set( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )

mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARY )
include_directories(${MYSQL_INCLUDE_DIR})
add_executable(judge_client okcalls.h ncalls.h okcalls32.h okcalls64.h json.hpp easywsclient.cpp easywsclient.hpp judge_client.cpp websocket.cpp websocket.h)
target_link_libraries(judge_client ${MYSQL_LIBRARIES})
