#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf { class RenderTarget; }

class b2Body;

namespace Game
{

class Planet
{
public:
    Planet();
    ~Planet();

    inline float getMass() const { return mMass; }
    inline float getRadius() const { return mRadius; }
    inline sf::Vector2f getPosition() const { return mPosition; }
    float getPercentage() const;
    inline bool getDirty() const { return mDirty; }
    inline void resetDirty() { mDirty = false; }

    void draw(sf::RenderTarget& target);

private:
    bool mDirty;
    float mMass;
    float mRadius;
    sf::Vector2f mPosition;
    b2Body* mBody;
};

}
