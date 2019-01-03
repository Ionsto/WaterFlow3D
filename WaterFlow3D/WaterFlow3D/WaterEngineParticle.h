#pragma once
#include "ParticleTree.h"
class WaterEngineParticle
{
private:
	ParticleTree ParticleList;
	void UpdateDensity();
	void UpdateDensityNode(ParticleNode & node,int x,int y, int z);
	void UpdateForces();
	void UpdateConditions();
	void Intergrate();
	float Poly6(float x);
public:
	WaterEngineParticle();
	~WaterEngineParticle();
};

