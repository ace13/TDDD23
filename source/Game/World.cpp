#include "World.hpp"
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
}