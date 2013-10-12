#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf { class RenderTarget; }

class b2Body;

namespace Game
{

class World;

class Planet
{
public:
    Planet();
    ~Planet();

    inline float getRadius() const { return mRadius; }
    inline sf::Vector2f getPosition() const { return mPosition; }
    inline void setPosition(const sf::Vector2f& pos) { mPosition = pos; }
    float getPercentage() const;
    inline bool getDirty() const { return mDirty; }
    inline void resetDirty() { mDirty = false; }

    void addedToWorld(World& world);
    
    void draw(sf::RenderTarget& target);

private:
    bool mDirty;
    float mRadius;
    sf::Vector2f mPosition;
    b2Body* mBody;
};

}
