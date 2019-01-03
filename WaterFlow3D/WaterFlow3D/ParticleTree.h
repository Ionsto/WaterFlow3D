#pragma once
#include <array>
#include "Vector.h"
struct Particle {
	Vector Velocity;
	float density = 0;
	static constexpr float Mass = 0;
};
class ParticleNode {
	static constexpr int MaxParticlesPerElement = 10;
	std::array<Particle, MaxParticlesPerElement> ParticleList;
};
class ParticleTree
{
public:
	static constexpr int Width = 20;
	static constexpr int Height = 1;
private:
	std::array<ParticleNode, (Width) * (Width) * (Height)> RawData;
public:
	ParticleTree();
	~ParticleTree();
	bool InBounds(int x, int y, int z) {
		return x >= 0 && x < Width && y >= 0 && y < Width &&  z >= 0 && z < Height;
	}
	ParticleNode GetNode(int x, int y, int z)
	{
		return RawData[(x * Width * Width) + (y * Width) + z];
	}
};

