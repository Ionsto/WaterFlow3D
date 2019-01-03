#pragma once
#include "ParticleTree.h"
class WaterEngineParticle
{
private:
	ParticleTree ParticleList;
	void UpdateDensity();
	void UpdateForces();
	void UpdateConditions();
	void Intergrate();
public:
	WaterEngineParticle();
	~WaterEngineParticle();
};

