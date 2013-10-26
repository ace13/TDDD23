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
    Ship(const Ship& rval);
    ~Ship();

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f&);
    float getAngle() const;
    inline float getRadius() const { return 10.f; }
    void setAngle(float);

    void setPlayer(Player*);
    inline Player* getPlayer() const { return mPlayer; }

    void addedToWorld(World&);

    void update(float dt);
    void draw(sf::RenderTarget& target);
    inline void collide(Entity& other) { };
    inline Type getType() const { return Type_Player; }

    inline float getHealth() const { return mHealth; }
    inline void setHealth(float h) { mHealth = h; }

    inline void setTurn(bool t = true) { mTurn = t; }

    void addGravity(const sf::Vector2f& pos, float strength);

private:
    friend class World;

    sf::Vector2f mPosition;
    float mAngle;
    float mFlyTime, mAnimTime;
    float mHealth;
    Player* mPlayer;
    b2Body* mBody;

    bool mTurn;

    int mGroup;

    friend class Weapon;
};

}
