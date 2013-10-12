#include "Planet.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

using namespace Game;

Planet::Planet() :
    mDirty(false), mMass(0), mRadius(0), mBody(nullptr)
{
}

Planet::~Planet()
{
    
}

float Planet::getPercentage() const
{
    return 0;
}

void Planet::draw(sf::RenderTarget& target)
{
    
}