#include "Application.hpp"
#include <SFML/Window/Event.hpp>

#include <ctime>

Application::Application(): mStateMan(*this), mScriptMan(*this), mOptions(*this)
{
}

Application::~Application()
{
}

void Application::init(int argc, char** argv)
{
    mAppName = argv[0];

    // Add options
    mOptions.addVariable<std::string>("resolution", "800x600", "The resolution of the window");
    mOptions.addVariable<bool>("fullscreen", false, "Run in a fullscreen window");

    mOptions.parseARGV(argc, argv);
    mScriptMan.init();
}

const Telemetry& Application::getTelemetry() const
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
    mLogger.log("Starting application", Logger::Info);

    {
        std::string res = mOptions.get<std::string>("resolution");
        unsigned int w = 800, h = 600;
        int r = sscanf(res.c_str(), "%4ux%4u", &w, &h);
        if (r != 2)
        {
            mLogger.log("Invalid resolution string '%s' specified, defaulting to %ux%u", Logger::Warning, w, h);
            w = 800; h = 600;
        }

        mWindow.create(sf::VideoMode(w, h), "TDDD23", (mOptions.get<bool>("fullscreen") ? sf::Style::Fullscreen : sf::Style::Default));
    }

    mUiView = mWindow.getDefaultView();

    float aspect;
    {
        sf::Vector2f s = (sf::Vector2f)mWindow.getSize();
        aspect = s.x / s.y;
    }

    mGameView = sf::View(sf::Vector2f(1000 * aspect, 1000), sf::Vector2f(0,0));

    auto updateView = [&]() {
        mUiView.setSize((sf::Vector2f)mWindow.getSize());
        mUiView.setCenter((sf::Vector2f)mWindow.getSize() / 2.f);
        mGameView.setSize(sf::Vector2f(1000 * aspect, 1000));

        auto uiS = mUiView.getSize(), uiC = mUiView.getCenter();
        auto gameS = mGameView.getSize(), gameC = mGameView.getCenter();

        mLogger.log("Window resized, view sizes:\n\tUI: %.2fx%.2f\n\tGame: %.2fx%.2f", Logger::Info, uiS.x, uiS.y, gameS.x, gameS.y);
    };

    unsigned int tickrate = 66;
    float msTick = 1.f / (float)tickrate;
    float totTime = 0;
    sf::Clock theTime;

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
