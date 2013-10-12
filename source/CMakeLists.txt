find_package(Box2D REQUIRED)
find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)
find_package(Angelscript REQUIRED COMPONENTS contextmgr debugger scriptany scriptarray scriptbuilder scriptdictionary scripthelper scriptmath scriptstdstring serializer)

include_directories(
    ${BOX2D_INCLUDE_DIR}
    ${SFML_INCLUDE_DIR}
    ${Angelscript_INCLUDE_DIR}
    ${Angelscript_ADDON_DIR}
)

set(DEBUG_SOURCE
    Debug/Console.hpp
    Debug/Console.cpp
    Debug/TelemetryOverlay.hpp
    Debug/TelemetryOverlay.cpp
)

set(GAME_SOURCE
    Game/GameState.hpp
    Game/GameState.cpp

    Game/Planet.hpp
    Game/Planet.cpp
    Game/Ship.hpp
    Game/Ship.cpp
    Game/Weapon.hpp
    Game/Weapon.cpp
    Game/World.hpp
    Game/World.cpp
)

set(MAIN_SOURCE
    Application.hpp
    Application.cpp
    Config.hpp
    IState.hpp
    Logger.hpp
    Logger.cpp
    Music.hpp
    Music.cpp
    Options.hpp
    Options.cpp
    ScriptManager.hpp
    ScriptManager.cpp
    StateManager.hpp
    StateManager.cpp
    Telemetry.hpp
    Telemetry.cpp
)

source_group(Script FILES ${Angelscript_HEADER} ${Angelscript_SOURCE})
source_group(Application FILES ${MAIN_SOURCE})
source_group(Debug FILES ${DEBUG_SOURCE})
source_group(Game FILES ${GAME_SOURCE})

add_executable(game main.cpp ${MAIN_SOURCE} ${DEBUG_SOURCE} ${GAME_SOURCE} ${Angelscript_HEADER} ${Angelscript_SOURCE})

target_link_libraries(game ${SFML_LIBRARIES} ${Angelscript_LIBRARIES} ${BOX2D_LIBRARY})
