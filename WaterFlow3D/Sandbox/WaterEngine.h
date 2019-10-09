#pragma once
#include "VoxelGrid.h"
#include "SwapList.h"
#include "Particle.h"
#include <fstream>
class WaterEngine
{
private:
	std::ofstream Logfile;
	double WeightPolyAxis(double x);
	double WeightPolyGradAxis(double x);
	double WeightPoly(glm::dvec2 distance);
	glm::dvec2 WeightGradPoly(glm::dvec2 distance);
	double WeightLinear(glm::dvec2 distance);
	glm::dvec2 WeightGradLinear(glm::dvec2 distance);

public:
	glm::dvec2 MousePull;
	double MouseAttract = 0;
	static const constexpr int MaxParticleCount = 2000;
	static const constexpr double DeltaTime = 1e-4;
	static constexpr int GridEvalSize = 1;
	int t = 0;
	double dtacc = 0;
	SwapList<Particle, MaxParticleCount> particle_list;
	VoxelGrid grid;
	WaterEngine();
	~WaterEngine();
	void PopulateNode(glm::dvec2 NodePos,Particle particle);
	void PopulateGrid();
	void UpdateParticlesNode(glm::dvec2 NodePos,Particle & particle);
	void UpdateParticles();
	void ResetGrid();
	void ApplyForces();
	void ApplyBoundary();
	void RemoveOutOfBounds();
	void Intergrate();
	void PreIntergrate();
	void IntergrateGrid();
	void IntergrateParticles();
	void Update(double dtreal);
	void AddWater(glm::dvec2 pos);
	void DumpParticles();
	float CalculateEnergy();
};

