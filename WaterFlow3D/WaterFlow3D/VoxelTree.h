#pragma once
#include <array>
struct VoxelData {
	char Type;
	char WaterVolume;
	char Direction;//x y z
};
class VoxelTree
{
public:
	static constexpr int Width = 512;
	static constexpr int Height = 64;
	std::array<VoxelData, Width * Width * Height> RawData;
	VoxelTree();
	~VoxelTree();
	VoxelData & GetValue(int x, int y, int z) {
		return RawData[z + (y * Width) + (x *Width*Width)];
	}
};

