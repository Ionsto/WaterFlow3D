#pragma once
#include "Vector.h"
struct Tensor {
	Vector DX;
	Vector DY;
};
struct Particle {
	float YoungsModulus = 1000;
	float ShearModulus = 10;
	int Type = 0;
	float Mass = 1;
	Vector Position;
	Vector PositionOld;
	Vector Velocity;
	Vector Force;
	Vector Momentum;
	Vector Acceleration;
	Vector Strain;
	Tensor Stress;
	Tensor StrainRate;
};