#pragma once

#include "Entity.hpp"
#include <memory>

namespace sf { class RenderTarget; class Shader; }

class b2Body;

namespace Game
{

class World;

class Planet : public Entity
{
public:
    Planet();
    ~Planet();

    inline float getRadius() const { return mRadius; }
    sf::Vector2f getPosition() const { return mPosition; }
    inline float getAngle() const { return 0; }
    void setPosition(const sf::Vector2f& pos);
    float getPercentage() const;
    inline bool getDirty() const { return mDirty; }
    inline void resetDirty() { mDirty = false; }

    void addedToWorld(World& world);
    inline void collide(Entity& other) { };
    
    void update(float) { }
    void draw(sf::RenderTarget& target);
    void drawWell(sf::RenderTarget& target);

private:
    bool mDirty;
    float mRadius;
    sf::Vector2f mPosition;
    b2Body* mBody;

    std::shared_ptr<sf::Shader> mWellShader;
};

}
