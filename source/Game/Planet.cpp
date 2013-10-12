#include "Planet.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <random>

using namespace Game;

Planet::Planet() :
    mDirty(false), mMass(0), mRadius(0), mBody(nullptr)
{
    std::random_device dev;
    std::uniform_real_distribution<float> dist(100, 500);
    
    mRadius = dist(dev);
    mPosition = sf::Vector2f(dist(dev), dist(dev));
}

Planet::~Planet()
{
    
}

float Planet::getPercentage() const
{
    return 1;
}

void Planet::draw(sf::RenderTarget& target)
{
    ///\TODO Better graphics :D
    sf::CircleShape shape(mRadius, 64);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(4.f);
    shape.setPosition(mPosition);

    shape.setOrigin(mRadius/2.f, mRadius/2.f);

    target.draw(shape);
}