#pragma once
#include <array>
#include "World.h"
#include <Eigen/Dense>
struct RenderData {
	char R, G, B;
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
};

