#pragma once
#include <glm/vec2.hpp>
struct Voxel
{
	double Mass = 0;
	glm::dvec2 Velocity;
	glm::dvec2 Force_Internal;
	glm::dvec2 Force_External;
	glm::dvec2 Force;
	glm::dvec2 Acceleration;
};

