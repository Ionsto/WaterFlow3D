#pragma once
#include "ParticleTree.h"
class WaterEngineParticle
{
private:
	static const constexpr float BoundaryDensity = 0.5;
	static const constexpr float DeltaTime = 0.01;
	static const constexpr float SmoothingParam = 7;
	static const constexpr float GasConstant = 500;
	static const constexpr float Density0 = 0.005;
	static const constexpr float Viscosity = 0.1;
	static const constexpr float Gravity = 9.81;
	static const constexpr float Restitution = 0.5;
	static const constexpr float Damping = 0.01;
	ParticleTree ParticleList;
	void UpdateDensity();
	void UpdateDensityNode(ParticleNode & node,int x,int y, int z);
	void UpdateForces();
	void UpdateForcesNode(ParticleNode & node,int x,int y, int z);
	void UpdateConditions();
	void Intergrate();
	void UpdateGrid();
	float Poly6(float x);
	float SpikyGrad(float r);
public:
	WaterEngineParticle();
	~WaterEngineParticle();
	void Update();
};

