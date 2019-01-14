#include "World.h"



World::World()
{
}


World::~World()
{
}

void World::Update()
{
	for (int x = 0; x < 1; ++x)
	{
		static constexpr int tmax = 1000;
		static constexpr float WaterMass = 2000;
		static constexpr float WaterFlowRate = 2000;
		if (t < 8/waterengine.DeltaTime && t % static_cast<int>(300 / (WaterMass * waterengine.DeltaTime)) == 0) {
			//waterengine.AddParticle(Vector(5 + (rand() % 10) / 15.0f, 9), WaterMass);
		}
		waterengine.Update();
		t++;
	}
}