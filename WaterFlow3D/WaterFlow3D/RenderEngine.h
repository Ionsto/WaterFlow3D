#pragma once
#include <array>
#include "World.h"
//#include <Eigen/Dense>
struct RenderData {
	char R, G, B;
	float Depth;
};
class RenderEngine
{
public:
	static constexpr int RenderWidth = 50;
	static constexpr int RenderHeight = 50;
	std::array<RenderData, RenderWidth * RenderHeight> RenderResult;
	
	RenderEngine();
	virtual ~RenderEngine();
	void Render(World & world);
	void RenderVoxelTree(VoxelTree & tree);
	void RenderVoxel(VoxelTree & tree);
};

