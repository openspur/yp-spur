cmake_minimum_required(VERSION 2.8.3)

# Build samples

add_executable(cartesian2d-test
	cartesian2d-test.c
)
target_link_libraries(cartesian2d-test
	carte2d${TARGET_STATIC_POSTFIX}
)

add_executable(MOI-estimate
	MOI-estimate.c
)
target_link_libraries(MOI-estimate
	ypspur${TARGET_STATIC_POSTFIX}
)

add_executable(run-test
	run-test.c
)
target_link_libraries(run-test
	ypspur${TARGET_STATIC_POSTFIX}
)
