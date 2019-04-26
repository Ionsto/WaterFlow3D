#pragma once
#include "Vector.h"
struct Voxel
{
	float Mass = 0;
	Vector Velocity;
	Vector Force_Internal;
	Vector Force_External;
	Vector Force;
	Vector Acceleration;
};

