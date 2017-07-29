cmake_minimum_required(VERSION 3.0.2)

# Build samples

add_executable(cartesian2d-test
	cartesian2d-test.c
)
target_link_libraries(cartesian2d-test
	carte2d
)

add_executable(MOI-estimate
	MOI-estimate.c
)
target_link_libraries(MOI-estimate
	ypspur
)

add_executable(run-test
	run-test.c
)
target_link_libraries(run-test
	ypspur
)
