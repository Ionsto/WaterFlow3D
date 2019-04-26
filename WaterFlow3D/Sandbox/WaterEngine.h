#pragma once
#include "VoxelGrid.h"
#include "SwapList.h"
#include "Particle.h"
class WaterEngine
{
public:
	static const constexpr int MaxParticleCount = 1000;
	static const constexpr float DeltaTime = 0.001f;
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
	void Intergrate();
	void IntergrateGrid();
	void IntergrateParticles();
	void Update(float dtreal);
	void AddWater(Vector pos);
};

