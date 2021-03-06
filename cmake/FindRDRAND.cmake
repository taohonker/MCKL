# ============================================================================
#  MCKL/cmake/FindRDRAND.cmake
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
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
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

# Find RDRAND support
#
# The following variable is set
#
# RDRAND_FOUND - TRUE if RDRAND is found and work correctly

IF(DEFINED RDRAND_FOUND)
    RETURN()
ENDIF(DEFINED RDRAND_FOUND)

FILE(READ ${CMAKE_CURRENT_LIST_DIR}/FindRDRAND.cpp RDRAND_TEST_SOURCE)

INCLUDE(CheckCXXSourceRuns)
CHECK_CXX_SOURCE_RUNS("${RDRAND_TEST_SOURCE}" RDRAND_FOUND)
IF(RDRAND_FOUND)
    MESSAGE(STATUS "Found RDRAND support")
ELSE(RDRAND_FOUND)
    MESSAGE(STATUS "NOT Found RDRAND support")
ENDIF(RDRAND_FOUND)
