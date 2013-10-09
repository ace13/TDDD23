#include "TelemetryOverlay.hpp"
#include "../Application.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

TelemetryOverlay::TelemetryOverlay()
{

}

TelemetryOverlay::~TelemetryOverlay()
{

}


void TelemetryOverlay::load()
{
    mFont = std::make_shared<sf::Font>();
    mFont->loadFromFile("Dosis-Book.ttf");
}

void TelemetryOverlay::unload()
{

}


void TelemetryOverlay::update(float dt)
{
    ///\TODO Move updating and stuff to here when amount of telemetry grows
}

void TelemetryOverlay::drawUi(sf::RenderTarget& target)
{
    char tmp[256];

    auto& ref = getApplication().getTelemetry();
    sprintf(tmp, "FPS (1,5,10):\n%f\n%f\n%f\nUPS: %d", ref.getFPS(), ref.getFPS(5), ref.getFPS(10), ref.getUPS());

    sf::Text debug(tmp, *mFont, 12U);
    debug.move(5, 5);
    target.draw(debug);
}
