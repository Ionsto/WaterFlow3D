#include "RenderSoftware.h"



RenderSoftware::RenderSoftware()
{
}


RenderSoftware::~RenderSoftware()
{
}
 
void RenderSoftware::Render(World & world) {
	for (int x = 0; x < world.tree->Width; ++x) {
		for (int y = 0; y < world.tree->Width; ++y) {
			for (int z = 0; z < world.tree->Width; ++z) {
				VoxelData & voxel = world.tree->GetValue(x, y, z);
			}
		}
	}
}