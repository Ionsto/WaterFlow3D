#pragma once
#include <array>
#include "Voxel.h"
#include <iostream>
class VoxelGrid
{
public:
	static constexpr int SizeX = 100;
	static constexpr int SizeY = 100;
	std::array<Voxel, SizeX * SizeY> Raw_Data;
	VoxelGrid();
	~VoxelGrid();
	bool InFlowBounds(int x, int y)
	{
		return x > 0 && y > 0 && x < SizeX - 1 && y < SizeY - 1;
	}
	bool InBounds(int x, int y)
	{
		return x >= 0 && y >= 0 && x < SizeX && y < SizeY;
	}
	Voxel& Get(int x, int y)
	{
		if (!InBounds(x,y)) {
			std::cout << "Out of grid access violation\n";
		}
		return Raw_Data[y + (x * SizeY)];
	}
};

