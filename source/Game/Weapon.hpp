#pragma once

#include "Entity.hpp"

namespace Game
{

class Weapon : public Entity
{
public:
    Weapon();
    ~Weapon();

    sf::Vector2f getPosition() const;
    float getAngle() const;

    void addedToWorld(Game::World& w);

    void update(float dt);
    void draw(sf::RenderTarget& target);

private:
};

}
