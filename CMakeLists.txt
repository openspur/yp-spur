cmake_minimum_required(VERSION 2.8.3)
project(formula-calc)

include_directories(include)

add_library(formula-calc STATIC src/libformula-calc.c)
target_link_libraries(formula-calc m)

add_executable(formula-test src/formula-test.c)
target_link_libraries(formula-test formula-calc)
add_dependencies(formula-test formula-calc)

install(TARGETS formula-calc
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
install(FILES include/formula-calc.h
	DESTINATION include
)

