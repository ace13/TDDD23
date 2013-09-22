find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

include_directories(${SFML_INCLUDE_DIR})

set(GAME_SOURCE
	Game/Ship.hpp
	Game/Ship.cpp
	Game/Weapon.hpp
	Game/Weapon.cpp
)

set(MAIN_SOURCE
	Application.hpp
	Application.cpp
	Logger.hpp
	Logger.cpp
	Telemetry.hpp
	Telemetry.cpp
)

source_group(Application FILES ${MAIN_SOURCE})
source_group(Game FILES ${GAME_SOURCE})

add_executable(game main.cpp ${MAIN_SOURCE} ${GAME_SOURCE})

target_link_libraries(game ${SFML_LIBRARIES})