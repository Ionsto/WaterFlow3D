#pragma once
#include "Vector.h"
struct Voxel
{
	float Mass;
	Vector Velocity;
	Vector Force_Internal;
	Vector Force_External;
	Vector Force;
};

