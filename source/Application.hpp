#pragma once

#include "Telemetry.hpp"
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

    Telemetry mTelem;
};
