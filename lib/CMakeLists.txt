# ============================================================================
#  MCKL/lib/CMakeLists.txt
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2016, Yan Zhou
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#    Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
# ============================================================================

PROJECT(MCKLLib CXX)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

SET(MCKL_LIB_SOURCE_MCKL
    ${PROJECT_SOURCE_DIR}/src/core/core.cpp
    ${PROJECT_SOURCE_DIR}/src/random/random.cpp
    ${PROJECT_SOURCE_DIR}/src/resample/resample.cpp
    ${PROJECT_SOURCE_DIR}/src/smp/smp.cpp
    ${PROJECT_SOURCE_DIR}/src/utility/utility.cpp)

SET(MCKL_LIB_TYPE shared static)
SET(MCKL_LIB_NAME mckl)

IF(NOT DEFINED MCKL_INSTALL_LIB_DIR)
    IF(EXISTS ${CMAKE_INSTALL_PREFIX}/lib64)
        SET(MCKL_INSTALL_LIB_DIR lib64)
    ELSE(EXISTS ${CMAKE_INSTALL_PREFIX}/lib64)
        SET(MCKL_INSTALL_LIB_DIR lib)
    ENDIF(EXISTS ${CMAKE_INSTALL_PREFIX}/lib64)
ENDIF(NOT DEFINED MCKL_INSTALL_LIB_DIR)

FOREACH(name ${MCKL_LIB_NAME})
    ADD_CUSTOM_TARGET(lib${name})
    ADD_DEPENDENCIES(lib lib${name})
    FOREACH(type ${MCKL_LIB_TYPE})
        STRING(TOUPPER "${name}" NAME)
        STRING(TOUPPER "${type}" TYPE)
        ADD_LIBRARY(lib${name}_${type} ${TYPE} ${MCKL_LIB_SOURCE_${NAME}})
        SET_TARGET_PROPERTIES(lib${name}_${type}
            PROPERTIES OUTPUT_NAME ${name})
        MCKL_LINK_TARGET(lib${name}_${type})
        ADD_DEPENDENCIES(lib${name} lib${name}_${type})
        INSTALL(TARGETS lib${name}_${type} DESTINATION ${MCKL_INSTALL_LIB_DIR})
    ENDFOREACH(type ${MCKL_LIB_TYPE})
ENDFOREACH(name ${MCKL_LIB_NAME})
