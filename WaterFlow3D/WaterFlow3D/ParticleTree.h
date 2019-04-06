#pragma once
#include <array>
#include "Vector.h"
#include <iostream>
#include "Particle.h"
class ParticleNode {
public:
	static constexpr int MaxParticlesPerElement = 10;
private:
	std::array<Particle, MaxParticlesPerElement> ParticleList;
public:
	int ParticleCount = 0;
	Particle & GetParticle(int i)
	{
		return ParticleList[i];
	}
	bool AddParticle(Particle add)
	{
		if (ParticleCount != MaxParticlesPerElement)
		{
			ParticleList[ParticleCount++] = add;
			return true;
		}
		std::cout << "Failed to add particle\n";
		return false;
	}
	void RemoveParticle(int position)
	{
		if (position == ParticleCount - 1)
		{
			ParticleCount--;
			return;
		}
		if (position < ParticleCount)
		{
			ParticleList[position] = ParticleList[ParticleCount-- - 1];
		}
		else {
			throw;
			//wtf
		}
	}
};
class ParticleTree
{
public:
	static constexpr int Width = 100;
	static constexpr int Height = 1;
	static constexpr const float Size = 1.2;
	static constexpr const float TotalWidth = Size * Width;
	static constexpr const float TotalHeight = Size * Height;
private:
	std::array<ParticleNode, (Width) * (Width) * (Height)> RawData;
public:
	ParticleTree();
	~ParticleTree();
	bool InBounds(int x, int y, int z) {
		return x >= 0 && x < Width && y >= 0 && y < Width &&  z >= 0 && z < Height;
	}
	ParticleNode & GetNode(int x, int y, int z)
	{
		return RawData[(x * Width * Height) + (y * Height) + z];
	}
	void AddParticle(Particle particle);
};

