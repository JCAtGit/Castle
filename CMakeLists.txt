cmake_minimum_required(VERSION 3.31)

cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0075 NEW)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project (
	CASTLE
	VERSION 1.0
	DESCRIPTION "C Assisted Source Translation Language Evaluator"
	)

	
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include(cmake/fetchcontent.cmake)
include(cmake/link_dependencies.cmake)
include(cmake/executables.cmake)
include(cmake/test_executables.cmake)
include(cmake/verbose.cmake)
