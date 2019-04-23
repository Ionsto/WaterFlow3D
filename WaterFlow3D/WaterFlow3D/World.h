#pragma once
#include "WaterEngineParticle.h"
#include "WaterEngineGPU.h"
class World
{
private:
	int WaterUpdateTicker = 0;
public:
	double DeltaTime = 0.016f;
	int t = 0;
	double dtacc = 0;
	//WaterEngineParticle waterengine;
	WaterEngineGPU waterengine;
	World();
	~World();
	void Update(float dt);
	void AddWater(Vector pos);
	void AddSand(Vector pos);
};

