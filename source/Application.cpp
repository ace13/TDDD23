#include "Application.hpp"
#include <SFML/Window/Event.hpp>

#include <ctime>

Application::Application(): mStateMan(*this), mScriptMan(*this)
{
}

Application::~Application()
{
}

void Application::init(int /*argc*/, char** /*argv*/)
{
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

void Application::run()
{
    mLogger.log("Starting application", Logger::Info);

    mWindow.create(sf::VideoMode(800, 600), "TDDD23");
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
                    aspect = s.x / s.y;
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
