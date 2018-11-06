#pragma once
#include "VoxelTree.h"
class World
{
public:
	std::unique_ptr<VoxelTree> tree;
	World();
	~World();
};

