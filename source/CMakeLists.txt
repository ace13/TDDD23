find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

include_directories(${SFML_INCLUDE_DIR})

set(SOURCE
	main.cpp
)

add_executable(game ${SOURCE})

target_link_libraries(game ${SFML_LIBRARIES})