find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
find_package(Angelscript REQUIRED COMPONENTS contextmgr debugger scriptany scriptarray scriptbuilder scriptdictionary scripthelper scriptmath scriptstdstring serializer)

include_directories(
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
    Game/Planet.hpp
    Game/Planet.cpp
    Game/Ship.hpp
    Game/Ship.cpp
    Game/Weapon.hpp
    Game/Weapon.cpp
)

set(MAIN_SOURCE
    Application.hpp
    Application.cpp
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

target_link_libraries(game ${SFML_LIBRARIES} ${Angelscript_LIBRARIES})
