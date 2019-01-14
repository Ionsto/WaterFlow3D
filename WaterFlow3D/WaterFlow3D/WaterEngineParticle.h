#pragma once
#include "ParticleTree.h"
#include <iostream>
class WaterEngineParticle
{
public:
	static const constexpr float DeltaTime = 0.01;
private:
	static const constexpr float BoundaryDensity = 0.5;
	static const constexpr float BoundaryFriction = 0.99;
	static const constexpr float SmoothingParam = 2;
	static const constexpr float GasConstant = 100;
	static const constexpr float Density0 = 1000;
	static const constexpr float Viscosity = 100000;
	static const constexpr float Gravity = 9.81;
	static const constexpr float Restitution = 0.5;
	static const constexpr float Damping = 0.001;
	void UpdateDensity();
	void UpdateDensityNode(ParticleNode & node,int x,int y, int z);
	void UpdateForces();
	void UpdateForcesNode(ParticleNode & node,int x,int y, int z);
	void UpdateFriction();
	void UpdateFrictionNode(ParticleNode & node);
	void UpdateConditions();
	void UpdateConditionsNode(ParticleNode & node);
	void Intergrate();
	void UpdateGrid();
	float Poly6(float x);
	float SpikyGrad(float r);
	float LaplaceVisc(float r);
public:
	ParticleTree ParticleList;
	WaterEngineParticle();
	~WaterEngineParticle();
	void Update();
	void AddParticle(Vector pos,float mass = 1);
	void AddParticle(Particle p);
	void Print(int t, std::ofstream & out);
};

