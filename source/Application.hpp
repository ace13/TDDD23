#pragma once

#include "Util/Logger.hpp"
#include "Util/Options.hpp"
#include "ScriptManager.hpp"
#include "StateManager.hpp"
#include "Util/Telemetry.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
public:
    Application();
    ~Application();

    void init(int argc, char** argv);
    void run();

    inline Options& getOptions() { return mOptions; }
    inline ScriptManager& getScript() { return mScriptMan; }
    Telemetry& getTelemetry();
    Logger& getLogger();
    std::string getApplicationName() const;

private:
    friend class StateManager;

    std::string mAppName;

    sf::RenderWindow mWindow;
    sf::View mGameView, mUiView;

    ScriptManager mScriptMan;
    StateManager mStateMan;
    Telemetry mTelem;
    Options mOptions;
    Logger mLogger;
};
