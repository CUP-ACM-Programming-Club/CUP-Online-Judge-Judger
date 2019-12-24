cmake_minimum_required(VERSION 3.9)
project(judge_client)
set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
if (EXISTS "/usr/local/bin/gcc-8")
    set(CMAKE_C_COMPILER /usr/local/bin/gcc-8)
    set(CMAKE_CXX_COMPILER /usr/local/bin/g++-8)
elseif (EXISTS "/usr/local/bin/gcc-9")
    set(CMAKE_C_COMPILER /usr/local/bin/gcc-9)
    set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9)
elseif (EXISTS "/usr/local/bin/gcc")
    set(CMAKE_C_COMPILER /usr/local/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/local/bin/g++)
else ()
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif ()
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
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
find_package(Threads)

set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARY)
include_directories(${MYSQL_INCLUDE_DIR})
add_executable(judge_client header/okcalls.h header/ncalls.h header/okcalls32.h header/okcalls64.h library/json.hpp library/easywsclient.cpp library/easywsclient.hpp judge_client.cpp model/websocket/WebSocketSender.cpp model/websocket/WebSocketSender.h library/judge_lib.h header/static_var.h model/base/JSONVectorReader.h model/base/JSONVectorReader.cpp model/base/Bundle.cpp model/base/Bundle.h library/judge_lib.h.cpp model/base/MySQLAutoPointer.cpp model/base/MySQLAutoPointer.h header/static_var.cpp model/base/Pack.cpp model/base/Pack.h model/wrapper/CompilerConfigReader.cpp model/wrapper/CompilerConfigReader.h model/base/ThreadPool.h model/SubmissionInfo.cpp model/SubmissionInfo.h model/judge/language/Language.cpp model/judge/language/Language.h model/judge/language/C11.cpp model/judge/language/C11.h model/judge/language/Cpp17.cpp model/judge/language/Cpp17.h model/judge/language/Pascal.cpp model/judge/language/Pascal.h model/judge/language/Objc.cpp model/judge/language/Objc.h model/judge/language/FreeBasic.cpp model/judge/language/FreeBasic.h model/judge/language/Clang.cpp model/judge/language/Clang.h model/judge/language/Clangpp.cpp model/judge/language/Clangpp.h model/judge/language/Clang11.cpp model/judge/language/Clang11.h model/judge/language/Clangpp17.cpp model/judge/language/Clangpp17.h model/judge/language/Go.cpp model/judge/language/Go.h model/judge/language/Cpp11.cpp model/judge/language/Cpp11.h model/judge/language/Cpp98.cpp model/judge/language/Cpp98.h model/judge/language/C99.cpp model/judge/language/C99.h model/judge/language/Java.cpp model/judge/language/Java.h model/judge/language/Java7.cpp model/judge/language/Java7.h model/judge/language/Java8.cpp model/judge/language/Java8.h model/judge/language/Java6.cpp model/judge/language/Java6.h model/judge/language/Ruby.cpp model/judge/language/Ruby.h model/judge/language/Bash.cpp model/judge/language/Bash.h model/judge/language/Python2.cpp model/judge/language/Python2.h model/judge/language/Php.cpp model/judge/language/Php.h model/judge/language/Perl.cpp model/judge/language/Perl.h model/judge/language/Csharp.cpp model/judge/language/Csharp.h model/judge/language/Schema.cpp model/judge/language/Schema.h model/judge/language/Lua.cpp model/judge/language/Lua.h model/judge/language/JavaScript.cpp model/judge/language/JavaScript.h model/judge/language/Python3.cpp model/judge/language/Python3.h model/judge/language/PyPy.cpp model/judge/language/PyPy.h model/judge/language/PyPy3.cpp model/judge/language/PyPy3.h)
target_link_libraries(judge_client ${MYSQL_LIBRARIES})
target_link_libraries(judge_client ${CMAKE_THREAD_LIBS_INIT})
