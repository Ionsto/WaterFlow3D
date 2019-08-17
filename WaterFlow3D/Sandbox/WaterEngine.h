#pragma once
#include "VoxelGrid.h"
#include "SwapList.h"
#include "Particle.h"
class WaterEngine
{
public:
	static const constexpr int MaxParticleCount = 1000;
	static const constexpr double DeltaTime = 0.0005f;
	int t = 0;
	double dtacc = 0;
	SwapList<Particle, MaxParticleCount> particle_list;
	VoxelGrid grid;
	WaterEngine();
	~WaterEngine();
	void PopulateNode(Vector NodePos,Particle particle);
	void PopulateGrid();
	void UpdateParticlesNode(Vector NodePos,Particle & particle);
	void UpdateParticles();
	void ResetGrid();
	void ApplyForces();
	void ApplyBoundary();
	void RemoveOutOfBounds();
	void Intergrate();
	void IntergrateGrid();
	void IntergrateParticles();
	void Update(double dtreal);
	void AddWater(Vector pos);
};

