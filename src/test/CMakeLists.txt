set(xcm-test_SOURCES

	xcm-test.cpp

)

add_executable(xcm-test ${xcm-test_SOURCES})
target_link_libraries(xcm-test ${xcm_LIBRARIES} xcmc)
add_test(xcm-test xcm-test)
