#include <array>
#include "Particle.h"
#include <fstream>
class Solver{
public:
	static const constexpr int MaxParticles = 4000;
	int DummyParticles = 0;
	static const constexpr float BoundaryDensity = 0.5;
	static const constexpr float DeltaTime = 0.01;
	static const constexpr float SmoothingParam = 7;
	static const constexpr float GasConstant = 500;
	static const constexpr float Density0 = 0.005;
	static const constexpr float Viscosity = 0.1;
	static const constexpr float Gravity = 9.81;
	static const constexpr float Restitution = 0.5;
	static const constexpr float Damping = 0.01;

	int ParticleCount = 0;
private:
	std::array<Particle,MaxParticles> ParticleSwapList;
	void UpdateConditions();
	void UpdatePressure();
	void UpdateForces();
	void Intergrate();
	float SmoothingKernal(float Radius);
	float SmoothingKernalGradient(float Radius);
	float Poly6(float r);
	float SpikyGrad(float r);
	float LaplaceVisc(float r);
public:
	Solver();// = default;
	~Solver() = default;
	void Update();
	void Print(int t,std::ofstream & out);
	Particle & GetParticle(int i)
	{
		return ParticleSwapList[i];
	}
	void AddParticle(Vector pos,float mass = 0.1);
};
