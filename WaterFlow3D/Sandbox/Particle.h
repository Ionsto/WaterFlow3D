#pragma once
#include "Vector.h"
#include <iostream>
#include <ostream>
struct Tensor {
	Vector DX;
	Vector DY;
};
struct Particle {
	float YoungsModulus = 1e5;
	float PoissonsRatio = 0.1;
	float ShearModulus = 1000;
	int Type = 0;
	float Mass = 1;
	Vector Position;
	Vector PositionOld;
	Vector Velocity;
	Vector Force;
	Vector Momentum;
	Vector Acceleration;
	//Vector Strain;
	Tensor Stress;
	Tensor Strain;
	Tensor StrainRate;
	friend std::ostream& operator<<(std::ostream& os, const Particle& p)
	{
		os	<< p.Position << ","
			<< p.Velocity << ","
			<< p.Force << ","
			<< p.Momentum << ","
			<< p.Stress.DX << ","
			<< p.Stress.DY << ","
			<< p.StrainRate.DX << ","
			<< p.StrainRate.DY;
		return os;
	}
};