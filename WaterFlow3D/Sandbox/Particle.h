#pragma once
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <ostream>
struct Particle {
	double YoungsModulus = -1e4;
	double PoissonsRatio = 0.0;
	double c1 = 0.3;
	double c2 = 0.0;
	double c3 = 0.0;
	double c4 = 0.0;
	int Type = 1;
	double Mass = 1e0;
	double Volume = 1;
	double StrainEnergy = 0;
	glm::dvec2 Position = glm::dvec2(0);
	glm::dvec2 PositionOld = glm::dvec2(0);
	glm::dvec2 Velocity = glm::dvec2(0);
	glm::dvec2 VelocityOld = glm::dvec2(0);
	glm::dvec2 Force = glm::dvec2(0);
	glm::dvec2 Momentum = glm::dvec2(0);
	glm::dvec2 Acceleration = glm::dvec2(0);
	glm::dvec2 AccelerationOld = glm::dvec2(0);
	glm::dmat2x2 Stress = glm::dmat2x2(0);
	static constexpr double StrainFactor = 1e0;
	glm::dmat2x2 Strain = glm::dmat2x2(0);
	glm::dmat2x2 StrainRate = glm::dmat2x2(0);
	glm::dmat2x2 StrainRateOld = glm::dmat2x2(0);
	friend std::ostream& operator<<(std::ostream& os, const Particle& p)
	{
/*		os	<< p.Position << ","
			<< p.Velocity << ","
			<< p.Force << ","
			<< p.Momentum << ","
			<< p.Stress << ","
			<< p.StrainRate << ","*/
		return os;
	}
};