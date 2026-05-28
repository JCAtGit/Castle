cmake_minimum_required(VERSION 3.22...3.31)

cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0075 NEW)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project (
	CASTLE
	VERSION 0.8
	DESCRIPTION "C Assisted Source Translation Language Evaluator"
	)
	
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/fetchcontent.cmake)
include(cmake/link_dependencies.cmake)
include(cmake/executables.cmake)
include(cmake/verbose.cmake)

#include(cmake/specific-compilations/test_c_project.cmake)