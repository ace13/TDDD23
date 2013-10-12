#include "LoadingScreen.hpp"
#include "../Config.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

LoadingScreen::LoadingScreen() :
    mTime(0)
{
   
}

LoadingScreen::~LoadingScreen()
{

}


bool LoadingScreen::load()
{
    mFont = std::make_shared<sf::Font>();
    mFont->loadFromFile("Dosis-Book.ttf");
    mSpinner = std::make_shared<sf::Texture>();
    mSpinner->loadFromFile("Spinner.png");
    mSpinner->setSmooth(true);

    return true;
}

void LoadingScreen::unload()
{
    mLastMessages.clear();
}

void LoadingScreen::update(float dt)
{
    mTime += dt;
}

void LoadingScreen::drawUi(sf::RenderTarget& target)
{
    target.clear(sf::Color::Black);

    sf::Text loadingText(mCurrentString, *mFont);
    sf::Sprite loadingSpinner(*mSpinner);

    {
        loadingSpinner.setRotation(mTime * 300);

        loadingSpinner.setOrigin((sf::Vector2f)mSpinner->getSize() / 2.f);
    }

    {
        sf::FloatRect tsize = loadingText.getLocalBounds();
        sf::Vector2f size = target.getView().getSize();
        loadingText.setOrigin(tsize.width / 2.f, tsize.height / 2.f);
        loadingSpinner.setPosition(size.x / 2.f, size.y / 2.f);
        loadingText.setPosition(size.x / 2.f, size.y - tsize.height * 2);
    }

    target.draw(loadingText);

    FOR_EACH(auto& text, mLastMessages)
    {
        loadingText.setString(text);

        sf::FloatRect tsize = loadingText.getLocalBounds();
        loadingText.setOrigin(tsize.width / 2.f, tsize.height / 2.f);
        loadingText.move(0, -tsize.height - 10);

        sf::Color c = loadingText.getColor();
        c.a /= 2;
        loadingText.setColor(c);

        target.draw(loadingText);
    }

    target.draw(loadingSpinner);
}


void LoadingScreen::setLoadingText(const std::string& text)
{
    if (text != mCurrentString && !mCurrentString.empty())
        mLastMessages.push_back(mCurrentString);

    mCurrentString = text;
}
