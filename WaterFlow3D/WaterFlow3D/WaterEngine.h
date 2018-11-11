#pragma once
#include "VoxelTree.h"
class WaterEngine
{
private:
	static constexpr float Gravity = 9.81;
	static constexpr float DeltaTime = 0.5;
public:
	WaterEngine();
	~WaterEngine();
	void EnforceBoundary(VoxelTree & tree);
	void EnforceInflows(VoxelTree & tree);
	void DiffuseDensity(VoxelTree & tree);
	void AdvectDensity(VoxelTree & tree);
	//float CalculateStress(VoxelTree & tree, int x, int y, int z);
	//void CalculateDensity(VoxelTree & tree, int x, int y, int z);
	////float CalculateVelocity(VoxelTree & tree, int x, int y, int z);
	//void UpdateHydrostaticStress(VoxelTree & tree);
	//void UpdateDensity(VoxelTree & tree);
	//void UpdateVelocity(VoxelTree & tree);
	//void UpdateSwapBuffer(VoxelTree & tree);
	void Update(VoxelTree & tree);
};

