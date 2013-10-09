#include "World.hpp"

using namespace Game;

World::World() : mWorld(nullptr)
{

}

World::~World()
{
	if (mWorld)
		delete mWorld;
}

void World::init()
{
	if (mWorld)
		return;

	mWorld = new b2World();
}

void World::update(float dt)
{
	if (!mWorld)
		return;

	static int velocitySteps = 8;
	static int positionSteps = 4;

	mWorld->step(dt, velocitySteps, positionSteps); ///\TODO Maybe check if this needs to be more or less static anywho
}