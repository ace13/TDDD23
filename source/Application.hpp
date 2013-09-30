#pragma once

#include "Logger.hpp"
#include "ScriptManager.hpp"
#include "StateManager.hpp"
#include "Telemetry.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
public:
    Application();
    ~Application();

    void init(int argc, char** argv);
    void run();

    const Telemetry& getTelemetry() const;
    Logger& getLogger();

private:
	friend class StateManager;

    sf::RenderWindow mWindow;

    sf::View mGameView, mUiView;

    ScriptManager mScriptMan;
    StateManager mStateMan;
    Telemetry mTelem;
    Logger mLogger;
};
