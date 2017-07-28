cmake_minimum_required(VERSION 3.0.2)
project(formula-calc)

include_directories(include)

add_library(formula-calc STATIC src/libformula-calc.c)
target_link_libraries(formula-calc m)

add_executable(formula-test src/formula-test.c)
target_link_libraries(formula-test formula-calc)
add_dependencies(formula-test formula-calc)

find_package(GTest)

if(GTEST_FOUND)
	enable_testing()
	
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)

	include_directories(${INCLUDE_DIRECTORIES} ${GTEST_INCLUDE_DIRS})
	add_executable(libformula-calc-test test/libformula-calc-test.cpp)
	target_link_libraries(libformula-calc-test formula-calc ${GTEST_LIBRARIES} Threads::Threads)

	add_test(AllTests libformula-calc-test)
endif(GTEST_FOUND)


install(TARGETS formula-calc
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
install(FILES include/formula-calc.h
	DESTINATION include
)

