#pragma once

#include "Entity.hpp"

class b2Body;

namespace Game
{

class Ship;

class Weapon : public Entity
{
public:
    Weapon(Ship* own = nullptr, float ang = 0.f, float force = 0.f);
    Weapon(const Weapon& other);
    ~Weapon();

    sf::Vector2f getPosition() const;
    float getAngle() const;

    void setOwner(Ship* own);

    void addedToWorld(Game::World& w);

    void update(float dt);
    void draw(sf::RenderTarget& target);
    void collide(Entity& other);

    void addGravity(const sf::Vector2f& pos, float strength);

    inline bool isDestroyed() const { return mDestroyed; }
    inline Type getType() const { return Type_Weapon; }

private:
    
    friend class World;

    Ship* mOwner;
    b2Body* mBody;

    float mFireAng, mFireForce;
    float mLiveTime;

    bool mDestroyed;
};

}
