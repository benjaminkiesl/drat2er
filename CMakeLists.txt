cmake_minimum_required(VERSION 3.5.1)
project (ci)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

# General options
include_directories(include)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include drat-trim as a library
add_library(drat-trim STATIC third-party/drat-trim/drat-trim.c)

# Build library for the main executable 
add_library(lib_drat2er STATIC 
	src/formula.cc
	src/formula_parser.cc
	src/clause.cc
	src/watch.cc
	src/rat_clause.cc
	src/rup_clause.cc
	src/deletion.cc
	src/lrat_parser.cc
	src/proof_transformer.cc
	src/rat_eliminator.cc
  	src/proof_stat_collector.cc
	src/deletion_eliminator.cc
	src/rup_to_resolution_transformer.cc
	src/proof_step_renamer.cc
	src/simple_parser.cc
	src/file_helper.cc
	src/drat_trim_interface.cc
	src/progress_bar.cc
	)
target_link_libraries(lib_drat2er drat-trim)

# Include the CLI11 command line parsing library
set(CLI11_INCLUDE_DIR third-party/cli11)
add_library(cli11 INTERFACE)
target_include_directories(cli11 INTERFACE ${CLI11_INCLUDE_DIR})

# Build the executable for the main program
add_executable(drat2er src/main.cc)
target_link_libraries(drat2er lib_drat2er cli11)

# Build the executable for the unit tests
set(CATCH_INCLUDE_DIR third-party/catch2)
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_DIRECTORY test) 
set(TEST_SOURCES ${TEST_DIRECTORY}/tests_main.cc
			 	 ${TEST_DIRECTORY}/test_clause.cc
				 ${TEST_DIRECTORY}/test_formula.cc
				 ${TEST_DIRECTORY}/test_lrat_parser.cc
				 ${TEST_DIRECTORY}/test_rup_clause.cc
				 ${TEST_DIRECTORY}/test_rat_clause.cc
				 ${TEST_DIRECTORY}/test_rat_eliminator.cc)
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests catch lib_drat2er)

# Enable Unit testing
enable_testing()
add_test(NAME UnitTests COMMAND unit_tests)
