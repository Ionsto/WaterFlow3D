#pragma once
#include "Vector.h"
struct Particle {
	bool Sand = true;
	Vector Position;
	Vector PositionOld;
	Vector Force;
	float Density = 0;
	float Pressure = 0;
	float GasConstant = 100;
	/*
	100000 sand
	*/
	float Viscosity = 100000;
	float Mass = 0;
	/*
	1000 sand and water
	*/
	float Density0 = 1000;
};
struct ParticleGPU {
	Vector Position;
	Vector PositionOld;
	Vector Force;
	float Pressure;
	float Density;
	//int Type;
};