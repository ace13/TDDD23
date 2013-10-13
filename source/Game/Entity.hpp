#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf { class RenderTarget; }

namespace Game { class World; }

class Entity
{
public:
    virtual sf::Vector2f getPosition() const = 0;
    virtual float getAngle() const = 0;

    virtual void addedToWorld(Game::World& w) = 0;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target) = 0;
};
