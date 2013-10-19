#pragma once

#include "Entity.hpp"

class b2Body;

namespace sf { class RenderTarget; }

namespace Game
{

class Player;
class Weapon;

class Ship : public Entity
{
public:
    Ship();
    ~Ship();

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f&);
    float getAngle() const;
    void setAngle(float);

    void addedToWorld(World&);

    void update(float dt);
    void draw(sf::RenderTarget& target);
    inline void collide(Entity& other) { };

    void addGravity(const sf::Vector2f& pos, float strength);

private:
    sf::Vector2f mPosition;
    float mAngle;
    Player* mPlayer;
    b2Body* mBody;

    int mGroup;

    friend class Weapon;
};

}
