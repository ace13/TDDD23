#pragma once

#include "Entity.hpp"

class b2Body;

namespace Game
{

class Ship;

class Weapon : public Entity
{
public:
    Weapon(Ship& own, float ang, float force);
    ~Weapon();

    sf::Vector2f getPosition() const;
    float getAngle() const;

    void addedToWorld(Game::World& w);

    void update(float dt);
    void draw(sf::RenderTarget& target);
    void collide(Entity& other);

    void addGravity(const sf::Vector2f& pos, float strength);

private:
    Ship& mOwner;
    b2Body* mBody;

    float mFireAng, mFireForce;
    float mLiveTime;
};

}
