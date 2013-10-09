#pragma once

#include "Entity.hpp"

class b2World;

namespace Game
{

class World
{
public:
	World();
	~World();

	void init();
	void update(float dt);

private:
	World(const World&);
	World& operator=(const World& other);

	b2World* mBox2DWorld;
};

}
