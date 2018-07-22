cmake_minimum_required(VERSION 3.5.1)
project (ci)

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

# General options
include_directories(include)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build library for the CI exercises
add_library(lib_drat2er STATIC 
	src/formula.cc
	src/formula_parser.cc
	src/clause.cc
	src/rat_eliminator.cc
	src/rat_clause.cc
	src/rup_clause.cc
	src/lrat_parser.cc
  	src/proof_stat_collector.cc
	)

# Build the executable for the main program
add_executable(drat2er src/main.cc)
target_link_libraries(drat2er lib_drat2er)

## Build unit tests
#enable_testing()
##add_subdirectory(${CMAKE_BINARY_DIR}/test/src
##				 ${CMAKE_BINARY_DIR}/test/build
##				 EXCLUDE_FROM_ALL)
#add_executable(formula_parser_test test/src/formula_parser_test.cc)
#
#target_link_libraries(formula_parser_test gtest_main lib_satyle)
#
#add_test(NAME FormulaParserTest COMMAND lib_satyle)
