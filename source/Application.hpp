#pragma once

#include "StateManager.hpp"
#include "Telemetry.hpp"
#include "Logger.hpp"
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

    StateManager mStateMan;
    Telemetry mTelem;
    Logger mLogger;
};
