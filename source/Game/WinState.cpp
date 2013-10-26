#include "WinState.hpp"
#include "../Application.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>

WinState::WinState(const Game::Player& winner) : mWinner(winner)
{
}
WinState::~WinState()
{
}

bool WinState::load()
{
    mFont = std::make_shared<sf::Font>();
    mFont->loadFromFile("Dosis-Book.ttf");

    return true;
}

void WinState::unload()
{

}

bool WinState::event(const sf::Event& ev)
{
    if (ev.type == sf::Event::KeyPressed)
    {
        getManager().popState();
        getApplication().getWindow().close();
    }

    return false;
}
void WinState::drawUi(sf::RenderTarget& target)
{
    sf::Text text;

    text.setFont(*mFont);
    text.setString("The winner is:");
    {
        auto size = target.getView().getSize();
        text.setPosition(size.x / 2.f, size.y / 2.f);
    }

    {
        auto size = text.getLocalBounds();
        text.setOrigin(size.width / 2.f, size.height / 2.f + size.height);
    }

    target.draw(text);

    text.setString(mWinner.getName());

    {
        auto size = target.getView().getSize();
        text.setPosition(size.x / 2.f, size.y / 2.f);
    }

    {
        auto size = text.getLocalBounds();
        text.setOrigin(size.width / 2.f, size.height / 2.f - size.height);
    }
    target.draw(text);

    text.setString("Press any key to exit");
    {
        auto size = target.getView().getSize();
        text.setPosition(size.x / 2.f, size.y - 16);
    }

    {
        auto size = text.getLocalBounds();
        text.setOrigin(size.width / 2.f, size.height / 2.f);
    }

    target.draw(text);
}