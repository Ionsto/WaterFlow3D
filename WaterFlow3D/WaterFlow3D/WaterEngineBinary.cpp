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
					tree.GetValueNew(x, y, z).Pressure += VoxelData::AirDensity * Gravity * DeltaTime / 6;
				}
				if (tree.GetValue(x, y, z).Type == VoxelData::VoxelType::Water) {
					tree.GetValueNew(x, y, z).Pressure += VoxelData::WaterDensity * Gravity * DeltaTime / 6;
				}
				if (tree.GetValue(x, y, z).Type == VoxelData::VoxelType::Sand) {
					tree.GetValueNew(x, y, z).Pressure += VoxelData::SandDensity * Gravity * DeltaTime / 6;
				}
			}
		}
	}
}
void WaterEngineBinary::UpdateInflows(VoxelTree & tree)
{
	tree.GetValue(2, 2, 0).Type = VoxelData::VoxelType::Water;
	//tree.GetValue(2, 2, 0).Pressure += 5;
}

void WaterEngineBinary::UpdateMovement(VoxelTree & tree)
{
	for (int x = 0; x < tree.Width; x++) {
		for (int y = 0; y < tree.Width; y++) {
			for (int z = 0; z < tree.Height; z++) {
				if (tree.GetValue(x, y, z).Type == VoxelData::VoxelType::Air)
				{
					Vector3F MinSide(tree.GetValue(x - 1, y, z).Pressure,
						tree.GetValue(x, y - 1, z).Pressure,
						tree.GetValue(x, y, z - 1).Pressure);
					Vector3F MaxSide(tree.GetValue(x + 1, y, z).Pressure,
						tree.GetValue(x, y + 1, z).Pressure,
						tree.GetValue(x, y, z + 1).Pressure);
					Vector3F Resultant = MinSide - MaxSide;
					if (tree.GetValue(x - 1, y, z).Type == VoxelData::VoxelType::Boundary) {
						Resultant.X = std::max(Resultant.X, 0.0f);
					}
					if (tree.GetValue(x + 1, y, z).Type == VoxelData::VoxelType::Boundary) {
						Resultant.X = std::min(Resultant.X, 0.0f);
					}
					if (tree.GetValue(x, y - 1, z).Type == VoxelData::VoxelType::Boundary) {
						Resultant.Y = std::max(Resultant.Y, 0.0f);
					}
					if (tree.GetValue(x, y + 1, z).Type == VoxelData::VoxelType::Boundary) {
						Resultant.Y = std::min(Resultant.Y, 0.0f);
					}
					if (tree.GetValue(x, y, z - 1).Type == VoxelData::VoxelType::Boundary) {
						Resultant.Z = std::max(Resultant.Z, 0.0f);
					}
					if (tree.GetValue(x, y, z + 1).Type == VoxelData::VoxelType::Boundary) {
						Resultant.Z = std::min(Resultant.Z, 0.0f);
					}
					float MaxForce = std::max({ MinSide.X,MinSide.Y,MinSide.Z, MaxSide.X,MaxSide.Y,MaxSide.Z, });
					if (MaxForce != 0) {
						int  i = 0;
					}
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
					if (MaxSide.X > ForceThreshold && MaxSide.X == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x + 1, y, z).Type = VoxelData::VoxelType::Air;
						continue;
					}
					if (MaxSide.Y > ForceThreshold && MaxSide.Y == MaxForce) {
						tree.GetValueNew(x, y, z).Type = VoxelData::VoxelType::Water;
						tree.GetValueNew(x, y + 1, z).Type = VoxelData::VoxelType::Air;
						continue;
					}
					if (MaxSide.Z > ForceThreshold && MaxSide.Z == MaxForce) {
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
	UpdateInflows(tree);
	UpdateMovement(tree);
	tree.SwapBuffer();
}
