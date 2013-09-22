#include "Application.hpp"
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics/Text.hpp>

Application::Application()
{
}

Application::~Application()
{
}

void Application::init(int argc, char** argv)
{

}

void Application::run()
{
    mWindow.create(sf::VideoMode(800, 600), "TDDD23");

    sf::Font debugFont;
    debugFont.loadFromFile("Dosis-Book.ttf");
    sf::Text debug;
    debug.setFont(debugFont);
    debug.setCharacterSize(18);

    sf::Event ev;
    while (mWindow.isOpen())
    {
        mTelem.startFrame();

        while (mWindow.pollEvent(ev))
        {
            switch(ev.type)
            {
            case sf::Event::Closed:
                mWindow.close();
                break;
            }
        }

        mWindow.clear();

        mWindow.draw(debug);

        mWindow.display();

        mTelem.endFrame();

        char tmp[256];
        float f1, f5, f10;
        f1 = mTelem.getFPS();
        f5 = mTelem.getFPS(5);
        f10 = mTelem.getFPS(10);

        sprintf(tmp, "%f\n%f\n%f", f1, f5, f10);

        debug.setString(tmp);
    }
}