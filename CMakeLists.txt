cmake_minimum_required(VERSION 3.5.1)
project (ci)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_BUILD_TYPE Debug)

# General options
include_directories(include)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build library for the main executable 
add_library(lib_drat2er STATIC 
	src/formula.cc
	src/formula_parser.cc
	src/clause.cc
	src/rat_eliminator.cc
	src/rat_clause.cc
	src/rup_clause.cc
	src/lrat_parser.cc
  	src/proof_stat_collector.cc
	src/deletion_eliminator.cc
	)

# Build the executable for the main program
add_executable(drat2er src/main.cc)
target_link_libraries(drat2er lib_drat2er)

# Build the executable for the unit tests
set(CATCH_INCLUDE_DIR third-party/catch2)
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_DIRECTORY test) 
set(TEST_SOURCES ${TEST_DIRECTORY}/tests_main.cc
				 ${TEST_DIRECTORY}/test_rat_eliminator.cc
			 	 ${TEST_DIRECTORY}/test_clause.cc
				 ${TEST_DIRECTORY}/test_rup_clause.cc
				 ${TEST_DIRECTORY}/test_rat_clause.cc)
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests catch lib_drat2er)

# Enable Unit testing
enable_testing()
add_test(NAME UnitTests COMMAND unit_tests)
