#include <array>
#include "Particle.h"
class Solver{
public:
	static const constexpr int MaxParticles = 1000;
	static const constexpr float DeltaTime = 0.1;
	static const constexpr float SmoothingParam = 4;
	static const constexpr float GasConstant = 1;
	static const constexpr float Density0 = 1;
	static const constexpr float Gravity = 9.81;
	static const constexpr float Restitution = 0.5;
	int ParticleCount = 0;
private:
	std::array<Particle,MaxParticles> ParticleSwapList;
	void UpdateConditions();
	void UpdatePressure();
	void UpdateForces();
	void Intergrate();
	float SmoothingKernal(float Radius);
	float SmoothingKernalGradient(float Radius);
public:
	Solver() = default;
	~Solver() = default;
	void Update();
	void Print(int t);
	Particle & GetParticle(int i)
	{
		return ParticleSwapList[i];
	}
	void AddParticle(Vector pos);
};
