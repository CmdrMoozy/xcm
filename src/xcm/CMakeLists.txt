set(xcm_SOURCES

	xcm.cpp

)

add_executable(xcm ${xcm_SOURCES})
target_link_libraries(xcm ${xcm_LIBRARIES})
