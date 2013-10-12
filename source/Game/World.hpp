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

private:
    World(const World&);
    World& operator=(const World& other);

    std::vector<Planet> mPlanets;
    std::vector<Ship> mShips;

    b2World* mBox2DWorld;
};

}
