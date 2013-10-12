#include "World.hpp"
#include "Planet.hpp"
#include "Ship.hpp"
#include "../Config.hpp"

#include <Box2D/Box2D.h>

using namespace Game;

World::World() : mBox2DWorld(nullptr)
{

}

World::~World()
{
    if (mBox2DWorld)
        delete mBox2DWorld;
}

void World::init()
{
    if (mBox2DWorld)
        return;

    mBox2DWorld = new b2World(b2Vec2(0,0));
}

void World::update(float dt)
{
    if (!mBox2DWorld)
        return;

    static int velocitySteps = 8;
    static int positionSteps = 4;

    mBox2DWorld->Step(dt, velocitySteps, positionSteps); ///\TODO Maybe check if this needs to be more or less static anywho

    FOR_EACH (auto& p, mPlanets)
    {
        if (p.getDirty())
        {
            p.resetDirty();

            ///\TODO Calculate if planet needs to be replaced.
        }
    }
}

void World::draw(sf::RenderTarget& target)
{
    FOR_EACH (auto& p, mPlanets)
    {
        p.draw(target);
    }
}

void World::addPlanet(const Planet& p)
{
    mPlanets.push_back(p);
}

void World::addShip(const Ship& s)
{
    mShips.push_back(s);
}