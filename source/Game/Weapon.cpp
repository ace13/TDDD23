#include "Weapon.hpp"

using namespace Game;

Weapon::Weapon()
{

}

Weapon::~Weapon()
{

}


sf::Vector2f Weapon::getPosition() const
{
    return sf::Vector2f();
}

float Weapon::getAngle() const
{
    return 0;
}


void Weapon::addedToWorld(Game::World& w)
{

}


void Weapon::update(float dt)
{

}

void Weapon::draw(sf::RenderTarget& target)
{

}
