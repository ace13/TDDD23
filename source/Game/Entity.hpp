#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf { class RenderTarget; }

namespace Game { class World; }

class Entity
{
public:
    enum Type
    {
        Type_Terrain,
        Type_Player,
        Type_Weapon
    };

    virtual sf::Vector2f getPosition() const = 0;
    virtual float getAngle() const = 0;
    virtual float getRadius() const = 0;

    virtual void addedToWorld(Game::World& w) = 0;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target) = 0;

    virtual void collide(Entity& other) = 0;
    virtual Type getType() const = 0;

protected:
    inline Game::World& getWorld() const { return *mWorld; }
    inline void setWorld(Game::World& w) { mWorld = &w; }

private:
    friend class Game::World;
    void addToWorld(Game::World& world) { mWorld = &world; addedToWorld(world);; }

    Game::World* mWorld;
};
