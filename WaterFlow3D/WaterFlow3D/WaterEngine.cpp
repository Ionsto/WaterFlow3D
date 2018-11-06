#include "WaterEngine.h"



WaterEngine::WaterEngine()
{
}


WaterEngine::~WaterEngine()
{
}

void WaterEngine::Update(VoxelTree & tree)
{
	for (int x = 0; x < tree.Width; ++x) {
		for (int y = 0; y < tree.Width; ++y) {
			for (int z = 0; z < tree.Width; ++z) {
				VoxelData & voxel = tree.GetValue(x, y, z);
			}
		}
	}
}