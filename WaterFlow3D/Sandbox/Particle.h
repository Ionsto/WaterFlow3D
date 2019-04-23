#pragma once
#include "Vector.h"
struct Particle {
	int Type = 0;
	float Mass = 1;
	Vector Position;
	Vector PositionOld;
	Vector Stress;
	Vector Force;
	Vector Momentum;
	Vector Acceleration;
};