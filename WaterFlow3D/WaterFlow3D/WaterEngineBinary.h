#pragma once
#include "VoxelTree.h"
class WaterEngineBinary
{
private:
	static constexpr float Gravity = 9.81;
	static constexpr float DeltaTime = 0.5;
	static constexpr float ForceThreshold = 1;
public:
	WaterEngineBinary();
	~WaterEngineBinary();
	void UpdateForcesVoxel(VoxelTree & tree, int x, int y, int z);
	void UpdateForces(VoxelTree & tree);
	void UpdateInflows(VoxelTree & tree);
	void UpdateMovement(VoxelTree & tree);
	void Update(VoxelTree & tree);
};

