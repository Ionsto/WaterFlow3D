#pragma once
#include <array>
class BSPNode{
	public:
		static const constexpr int MaxParticleCount = 20;
		int ParticleCount = 0;
		BSPNode() = default;
	private:
		std::array<Particle, MaxParticleCount> RawData;	
};
class ParticleBSP{
	public:
		static const constexpr int Size = 1;
		static const constexpr int Width = 50;
		static const constexpr int Height = 10;
	private:
		std::array<BSPNode,Width * Width * Height> RawData;
	public:
		ParticleBSP() = default;
	BSPNode & GetValue(int x, int y, int z) {
		return RawData[SwapBufferIndex][(z + 1) + ((y + 1) * (Height + 2)) + ((x + 1) * (Height + 2) * (Width + 2))];
	}
};
