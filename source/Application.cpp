#include "Application.hpp"
#include <SFML/Window/Event.hpp>

#include <ctime>

Application::Application(): mStateMan(*this)
{
}

Application::~Application()
{
}

void Application::init(int /*argc*/, char** /*argv*/)
{

}

void Application::run()
{
    mLogger.log(Logger::Info, ("Starting application"));

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

#if 0
        auto uiS = mUiView.getSize(), uiC = mUiView.getCenter();
        auto gameS = mGameView.getSize(), gameC = mGameView.getCenter();

        std::cout << "Updated views to:" << std::endl
            << "UI - " << uiS.x << "x" << uiS.y << " at " << uiC.x << ", " << uiC.y << std::endl
            << "Game - " << gameS.x << "x" << gameS.y << std::endl;
#endif
    };

    sf::Event ev;
    while (mWindow.isOpen())
    {
        mTelem.startFrame();

        while (mWindow.pollEvent(ev))
        {
            mStateMan.event(ev);

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

        mStateMan.update(1.f);

        mWindow.clear();

        mWindow.setView(mGameView);

        mStateMan.draw(mWindow);

        ///\TODO Draw game

        mWindow.setView(mUiView);

        mStateMan.drawUi(mWindow);

        mWindow.display();

        mTelem.endFrame();
    }
}
