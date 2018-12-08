#include <array>
#include "Particle.h"
class Solver{
public:
	static const constexpr int MaxParticles = 1000;
	static const constexpr int DeltaTime = 1;
private:
	std::array<Particle,MaxParticles> ParticleSwapList;
	int ParticleCount;
	void UpdateConditions();
	void UpdatePressure();
	void Intergrate();
public:
	Solver() = default;
	~Solver() = default;
	void Update();
	Particle & GetParticle(int i)
	{
		return ParticleSwapList[i];
	}
};
