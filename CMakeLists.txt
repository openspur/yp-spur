cmake_minimum_required(VERSION 3.0.2)
project(formula-calc)

include_directories(include)

add_library(formula-calc SHARED src/libformula-calc.c)
target_link_libraries(formula-calc m)
set_target_properties(formula-calc PROPERTIES
	IMPORT_SUFFIX "_import.lib"
	VERSION 0.0.1
	SOVERSION 0
	PUBLIC_HEADER include/formula-calc.h
)

add_library(formula-calc-static STATIC src/libformula-calc.c)
target_link_libraries(formula-calc-static m)
set_target_properties(formula-calc-static PROPERTIES
	OUTPUT_NAME "formula-calc"
	PUBLIC_HEADER include/formula-calc.h
)

add_executable(formula-test src/formula-test.c)
target_link_libraries(formula-test formula-calc)
add_dependencies(formula-test formula-calc)

find_package(GTest)

if(GTEST_FOUND)
	enable_testing()
	
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)

	find_program(VALGRIND
		NAMES valgrind
	)

	include_directories(${INCLUDE_DIRECTORIES} ${GTEST_INCLUDE_DIRS})
	add_executable(libformula-calc-test test/libformula-calc-test.cpp)
	target_link_libraries(libformula-calc-test formula-calc ${GTEST_LIBRARIES} Threads::Threads)

	add_test(UnitTests libformula-calc-test)

	if(VALGRIND)
		add_test(MemoryLeakTest
			${VALGRIND} --error-exitcode=1 --leak-check=full
			${CMAKE_CURRENT_BINARY_DIR}/libformula-calc-test
		)
	endif(VALGRIND)
endif(GTEST_FOUND)


install(TARGETS
		formula-calc
		formula-calc-static
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
	PUBLIC_HEADER DESTINATION include
)

