#pragma once
#include "ParticleTree.h"
#include <iostream>
#include <random>
class WaterEngineParticle
{
public:
	static const constexpr double DeltaTime = 0.005;
private:
	static const constexpr double BoundaryDensity = 0.2;
	static const constexpr double BoundaryFriction = 0.9;
	static const constexpr double SmoothingParam = 1.05;
	static const constexpr double SmoothingParamViscosity = 2;
	static const constexpr double GasConstant = 1000;
	static const constexpr double Density0 = 1000;
	static const constexpr double Viscosity = 100000;
	static const constexpr double Gravity = 9.81;
	static const constexpr double Restitution = 0.5;
	static const constexpr double Damping = 0.001;
	static const constexpr float TotalWidth = ParticleTree::TotalWidth;
	static const constexpr float TotalHeight = ParticleTree::TotalHeight;
	std::random_device rd;
	std::mt19937 distibution{rd()};
	std::normal_distribution<double> BrownianMotion{ 0, 200 };
	void UpdateDensity();
	void UpdateDensityNode(ParticleNode & node,int x,int y, int z);
	void UpdateForces();
	void UpdateForcesNode(ParticleNode & node,int x,int y, int z);
	void UpdateFriction();
	void UpdateFrictionNode(ParticleNode & node);
	void UpdateConditions();
	void UpdateConditionsNode(ParticleNode & node);
	void UpdateConditionsAxis(double &x, double &y, double &z, double &ox, double &oy, double &oz, double lim);
	void Intergrate();
	void UpdateGrid();
	double Poly6(double x);
	double SpikyGrad(double r);
	double LaplaceVisc(double r);
public:
	ParticleTree ParticleList;
	WaterEngineParticle();
	~WaterEngineParticle();
	void Update();
	void AddParticle(Vector pos,double mass = 1);
	void AddParticle(Particle p);
	void Print(int t, std::ofstream & out);
};

