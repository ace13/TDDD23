#pragma once

#include "Entity.hpp"
#include "../Config.hpp"
#include <vector>

namespace sf { class RenderTexture; }

class b2Draw;
class b2Body;
class b2World;

namespace Game
{

class Planet;
class Ship;

class World
{
public:
    World();
    ~World();

    void init();
    void update(float dt);
    void updateWalls();

    void addPlanet(const Planet& planet);
    void addShip(const Ship& ship);

    void draw(sf::RenderTarget& target);
    void drawUi(sf::RenderTarget& target);

    inline sf::Vector2f getSize() const { return mSize; }
    inline void setSize(sf::Vector2f size) { mSize = size; }

    inline b2World* getBox2D() { return mBox2DWorld; }

private:
    World(const World&);
    World& operator=(const World& other);

    sf::Vector2f mSize;

    std::vector<Planet> mPlanets;
    std::vector<Ship> mShips;

    b2Body* mWalls[4];
    b2World* mBox2DWorld;
#ifdef DEBUG
    b2Draw* mDebugDraw;
    sf::RenderTexture* mDebugTarget;
#endif
};

}
