#pragma once
#include "WaterEngineParticle.h"
class World
{
public:
	int t = 0;
	WaterEngineParticle waterengine;
	World();
	~World();
	void Update();
};

