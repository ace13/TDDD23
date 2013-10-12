#include "World.hpp"
#include "Planet.hpp"
#include "Ship.hpp"
#include "../Config.hpp"

#include <Box2D/Box2D.h>
#include <random>

using namespace Game;

World::World() : mBox2DWorld(nullptr)
{

}

World::~World()
{
    mPlanets.clear();
    mShips.clear();

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

///\FIXME Don't draw everything always.
void World::draw(sf::RenderTarget& target)
{
    FOR_EACH (auto& p, mPlanets)
    {
        p.draw(target);
    }

    FOR_EACH(auto& s, mShips)
    {
        s.draw(target);
    }
}

void World::addPlanet(const Planet& p)
{
    Game::Planet tmp = p;

    std::random_device dev;
    std::uniform_real_distribution<float> distX(0, mSize.x);
    std::uniform_real_distribution<float> distY(0, mSize.y);

    tmp.setPosition(sf::Vector2f(distX(dev) - mSize.x / 2.f, distY(dev) - mSize.y / 2.f));

    mPlanets.push_back(tmp);
}

void World::addShip(const Ship& s)
{
    mShips.push_back(s);
}