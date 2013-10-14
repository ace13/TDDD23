#include "Console.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Utf.hpp>

Console::Console()
{

}

Console::~Console()
{

}


bool Console::load()
{
    mFont = std::make_shared<sf::Font>();
    mFont->loadFromFile("Dosis-Book.ttf");

    return true;
}

void Console::unload()
{

}

std::string Console::getLoadState() const
{
    return "";
}


bool Console::event(const sf::Event& ev)
{
    if (ev.type == sf::Event::TextEntered)
    {
        auto utf32 = ev.text.unicode;
        if (utf32 < 128)
        {
            ///\TODO Non-ASCII inputs.

            char letter = (char)utf32;
            
            switch(letter)
            {
            case '\b':
                mCurrentLine = mCurrentLine.erase(mCurrentLine.length()-1, 1);
                break;

            case 13:
            case '\n':
                ///\TODO Use console input
                mCurrentLine.clear();
                break;

            default:
                mCurrentLine += letter;
            }
        }

        return true;
    }

    return false;
}

void Console::update(float dt)
{

}

void Console::drawUi(sf::RenderTarget& target)
{
    auto size = target.getView().getSize();
    sf::RectangleShape console(sf::Vector2f(size.x, std::min(256.f, size.y)));
    console.setFillColor(sf::Color(32, 32, 64, 200));

    sf::Text consoleText(mCurrentLine, *mFont, 16);

    target.draw(console);
    target.draw(consoleText);
}
