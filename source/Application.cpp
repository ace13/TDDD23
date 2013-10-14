#include "Application.hpp"
#include "Game/GameState.hpp"
#include "Config.hpp"
#include <SFML/Window/Event.hpp>

#include <ctime>

Application::Application(): mStateMan(*this), mScriptMan(*this), mOptions(*this), mLogger(*this)
{
    /// \TODO Redirect sf::err to Logger
}

Application::~Application()
{
}

void Application::init(int argc, char** argv)
{
#ifdef WINDOWS
    mAppName = argv[0];
    // mAppName = mAppName.substr() // Cut out to last backslash
#else
    mAppName = argv[0];
#endif

    // Add options
    mOptions.addVariable<std::string>("resolution", "800x600", "The resolution of the window");
    mOptions.addVariable<bool>("fullscreen", false, "Run in a fullscreen window");
    mOptions.addVariable<int>("tickrate", 66, "Update ticks per second");

    mOptions.parseARGV(argc, argv);
    mLogger.init();
    mScriptMan.init();
}

Telemetry& Application::getTelemetry()
{
    return mTelem;
}

Logger& Application::getLogger()
{
    return mLogger;
}

std::string Application::getApplicationName() const
{
    return mAppName;
}

void Application::run()
{
    mLogger.log("Starting application %s", Logger::Info, mAppName.c_str());

    {
        std::string res = mOptions.get<std::string>("resolution");
        unsigned int w = 800, h = 600;
        int r = sscanf(res.c_str(), "%4ux%4u", &w, &h);
        if (r != 2)
        {
            mLogger.log("Invalid resolution string '%s' specified, defaulting to %ux%u", Logger::Warning, w, h);
            w = 800; h = 600;
        }

        if (mOptions.get<bool>("fullscreen"))
        {
            auto mode = sf::VideoMode::getDesktopMode();
            w = mode.width;
            h = mode.height;
        }

        mWindow.create(sf::VideoMode(w, h), "TDDD23", (mOptions.get<bool>("fullscreen") ? sf::Style::None : sf::Style::Default));
    }

    mUiView = mWindow.getDefaultView();
    auto updateView = [&]() {
        auto size = (sf::Vector2f)mWindow.getSize();
        float aspect = size.x / size.y;

        mGameView.setSize(mGameView.getSize().y * aspect, mGameView.getSize().y);

        mUiView.setSize(size);
        mUiView.setCenter(size / 2.f);

        auto uiS = mUiView.getSize(), uiC = mUiView.getCenter();
        auto gameS = mGameView.getSize(), gameC = mGameView.getCenter();

        mLogger.log("Window resized, view sizes:\n\tUI: %.2fx%.2f\n\tGame: %.2fx%.2f", Logger::Info, uiS.x, uiS.y, gameS.x, gameS.y);
    };

    updateView();

    float msTick = 1.f / (float)mOptions.get<int>("tickrate");
    float totTime = 0;
    sf::Clock theTime;

    mStateMan.pushState(new GameState()); ///\TODO Menustate

    sf::Event ev;
    while (mWindow.isOpen())
    {
        mTelem.startFrame();

        totTime += theTime.restart().asSeconds();

        while (mWindow.pollEvent(ev))
        {
            if (mStateMan.doEvent(ev))
                continue;

            switch(ev.type)
            {
            case sf::Event::Closed:
                mWindow.close();
                break;
            case sf::Event::Resized:
                {
                    sf::Vector2f s = (sf::Vector2f)mWindow.getSize();
                }
                updateView();
                break;
            default:
                break;
            }
        }

        // Sanity check, if more than five updates are missed, then don't try to do them all.
        if (totTime > msTick * 5)
            totTime = msTick * 5;

        while (totTime >= msTick)
        {
            mTelem.startUpdate();

            mStateMan.doUpdate(msTick);
            totTime -= msTick;

            mTelem.endUpdate();
        }

        mWindow.clear();

        mWindow.setView(mGameView);

        mStateMan.doDraw(mWindow);

        mWindow.setView(mUiView);

        mStateMan.doDrawUi(mWindow);

        mWindow.display();

        mTelem.endFrame();
    }

    mLogger.log("Shutting down application", Logger::Info);
}

sf::Vector2f Application::getMouse(bool ui)
{
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(mWindow);
    sf::Vector2f vS, vC;

    if (ui)
    {
        vS = mUiView.getSize();
        vC = mUiView.getCenter();
    }
    else
    {
        vS = mGameView.getSize();
        vC = mGameView.getCenter();
    }

    sf::Vector2f windowSize = (sf::Vector2f)mWindow.getSize();
        
    sf::FloatRect mViewRect;
    mViewRect.left = vC.x-vS.x/2;
    mViewRect.top = vC.y-vS.y/2;
    mViewRect.width = vS.x;
    mViewRect.height = vS.y;

    return sf::Vector2f(mViewRect.left + mViewRect.width * (mousePos.x/windowSize.x), mViewRect.top + mViewRect.height * (mousePos.y/windowSize.y));
}