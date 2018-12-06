#include <array>
#include "Particle.h"
class Solver{
public:
	static const constexpr int MaxParticles = 1000;
private:
	std::array<Particle,MaxParticles> ParticleSwapList;
	int ParticleCount;
	void UpdateKernals();
	void Intergrate();
public:
	Solver() = default;
	~Solver() = default;
	void Update();
};
