cmake_minimum_required(VERSION 3.20...3.31)

cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0075 NEW)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project (
	DE_CompilationBenchmarker
	VERSION 0.2
	DESCRIPTION "A (De)Compilation Project"
	)
	
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(cmake/config.h.in config.h)
include(cmake/fetchcontent.cmake)
include(cmake/link_dependencies.cmake)
include(cmake/executables.cmake)
include(cmake/verbose.cmake)

#include(cmake/specific-compilations/test_c_project.cmake)