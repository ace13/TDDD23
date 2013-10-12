#pragma once

#include "Entity.hpp"
#include <vector>

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

    void addPlanet(const Planet& planet);
    void addShip(const Ship& ship);

    void draw(sf::RenderTarget& target);

    inline sf::Vector2f getSize() const { return mSize; }
    inline void setSize(sf::Vector2f size) { mSize = size; }

    inline b2World* getBox2D() { return mBox2DWorld; }

private:
    World(const World&);
    World& operator=(const World& other);

    sf::Vector2f mSize;

    std::vector<Planet> mPlanets;
    std::vector<Ship> mShips;

    b2World* mBox2DWorld;
};

}
