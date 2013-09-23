#pragma once

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

private:
    sf::RenderWindow mWindow;

    sf::View mGameView, mUiView;

    Telemetry mTelem;
    Logger mLogger;
};
