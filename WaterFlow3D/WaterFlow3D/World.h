#pragma once
#include "WaterEngineParticle.h"
class World
{
private:
	int WaterUpdateTicker = 0;
public:
	double DeltaTime = 1.0f / 200.0f;
	int t = 0;
	double dtacc = 0;
	WaterEngineParticle waterengine;
	World();
	~World();
	void Update(float dt);
	void AddWater(Vector pos);
	void AddSand(Vector pos);
};

