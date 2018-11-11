#include "WaterEngineBinary.h"



WaterEngineBinary::WaterEngineBinary()
{
}


WaterEngineBinary::~WaterEngineBinary()
{
}
void WaterEngineBinary::UpdateForcesVoxel(VoxelTree & tree, int x, int y, int z) {
	auto & Center = tree.GetValue(x, y, z);
	tree.GetValueNew(x, y, z).Pressure =
		tree.GetValue(x -1, y, z).Pressure +
		tree.GetValue(x + 1, y, z).Pressure +
		tree.GetValue(x, y - 1, z).Pressure +
		tree.GetValue(x, y + 1, z).Pressure +
		tree.GetValue(x, y, z - 1).Pressure +
		tree.GetValue(x, y, z + 1).Pressure;
}
void WaterEngineBinary::UpdateForces(VoxelTree & tree)
{
	for (int x = 0; x < tree.Width; x++) {
		for (int y = 0; y < tree.Width; y++) {
			for (int z = 0; z < tree.Height; z++) {
				//Update water
				auto & Center = tree.GetValue(x, y, z);
				tree.GetValueNew(x, y, z).Pressure =
					(tree.GetValue(x - 1, y, z).Pressure +
					tree.GetValue(x + 1, y, z).Pressure +
					tree.GetValue(x, y - 1, z).Pressure +
					tree.GetValue(x, y + 1, z).Pressure +
					tree.GetValue(x, y, z - 1).Pressure +
					tree.GetValue(x, y, z + 1).Pressure)/6;

				if (tree.GetValue(x, y, z).Type == VoxelData::VoxelType::Air) {
					tree.GetValueNew(x, y, z).Pressure += VoxelData::AirDensity * Gravity * DeltaTime;
				}
				if (tree.GetValue(x, y, z).Type == VoxelData::VoxelType::Water) {
					tree.GetValueNew(x, y, z).Pressure += VoxelData::WaterDensity * Gravity * DeltaTime;
				}
				if (tree.GetValue(x, y, z).Type == VoxelData::VoxelType::Sand) {
					tree.GetValueNew(x, y, z).Pressure += VoxelData::SandDensity * Gravity * DeltaTime;
				}
			}
		}
	}
}
void WaterEngineBinary::UpdateInflows(VoxelTree & tree)
{
	tree.GetValue(0,0,0).Type = VoxelData::VoxelType::Water;
}

void WaterEngineBinary::UpdateMovement(VoxelTree & tree)
{
	for (int x = 0; x < tree.Width; x++) {
		for (int y = 0; y < tree.Width; y++) {
			for (int z = 0; z < tree.Height; z++) {
				if (tree.GetValue(x, y, z).Type == VoxelData::VoxelType::Air)
				{
					Vector3F MinSide(tree.GetValue(x - 1, y, z).Force.X,
						tree.GetValue(x, y - 1, z).Force.Y,
						tree.GetValue(x, y, z - 1).Force.Z);
					Vector3F MaxSide(tree.GetValue(x + 1, y, z).Force.X,
						tree.GetValue(x, y + 1, z).Force.Y,
						tree.GetValue(x, y, z + 1).Force.Z);
					Vector3F Resultant = MinSide - MaxSide;
					float MaxForce = std::max({ MinSide.X,MinSide.Y,MinSide.Z, MaxSide.X,MaxSide.Y,MaxSide.Z, });
					if (MinSide.X > ForceThreshold && MinSide.X == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x - 1, y, z).Type = VoxelData::VoxelType::Air;
						continue;
					}
					if (MinSide.Y > ForceThreshold && MinSide.Y == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x, y - 1, z).Type = VoxelData::VoxelType::Air;
						continue;
					}
					if (MinSide.Z > ForceThreshold && MinSide.Z == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x, y, z - 1).Type = VoxelData::VoxelType::Air;
						continue;
					}
					if (MaxSide.X > ForceThreshold && MinSide.X == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x + 1, y, z).Type = VoxelData::VoxelType::Air;
						continue;
					}
					if (MaxSide.Y > ForceThreshold && MinSide.Y == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x, y + 1, z).Type = VoxelData::VoxelType::Air;
						continue;
					}
					if (MaxSide.Z > ForceThreshold && MinSide.Z == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x, y, z + 1).Type = VoxelData::VoxelType::Air;
						continue;
					}
				}
			}
		}
	}
}

void WaterEngineBinary::Update(VoxelTree & tree)
{
	UpdateInflows(tree);
	UpdateForces(tree);
	UpdateMovement(tree);
	tree.SwapBuffer();
}
