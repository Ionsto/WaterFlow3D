#pragma once
#include <array>
#include <vector>
#include "Vector.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Particle.h"
struct GPUScene {
	int ParticleCount = 0;
};
template
<class t, int max>
class SwapList {
public:
	static constexpr int MaxParticlesPerElement = max;
	std::array<t, MaxParticlesPerElement> ParticleList;
	int ParticleCount = 0;
	t & GetParticle(int i)
	{
		return ParticleList[i];
	}
	std::array<t, MaxParticlesPerElement> RawData()
	{
		return ParticleList;
	}
	bool AddParticle(t add)
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
class WaterEngineGPU
{
public:
	static const constexpr double DeltaTime = 0.100;
	static const constexpr float TotalWidth = 100;
	static const constexpr float TotalHeight = 100;
	int FlowCount = 0;
	GLFWwindow* Window_Handle;
	Shader ComputeTest;
	Shader Compute_Density;
	Shader Compute_Force;
	Shader Compute_Intergrate;
	Shader Compute_Flow;
	ShaderProgram TestProgram;
	ShaderProgram Program_Density;
	ShaderProgram Program_Force;
	ShaderProgram Program_Intergrate;
	ShaderProgram Program_Flow;
	GLuint PositionBuffer;
	GLuint PositionOldBuffer;
	GLuint DensityBuffer;
	GLuint PressureBuffer;
	GLuint ForceBuffer;
	GLuint TypeBuffer;
	GLuint SceneBuffer;
	static constexpr const int MaxParticleCount = 10000;
	int ParticleCount = 0;
	GPUScene GPUscene;
	SwapList<ParticleGPU, MaxParticleCount> ParticleList;
	std::array<float, MaxParticleCount * 4> GPUBufferPosition = { 10 };
	std::array<float, MaxParticleCount * 4> GPUBufferPositionOld = { 10 };
	std::array<float, MaxParticleCount> GPUBufferDensity;
	std::array<float, MaxParticleCount> GPUBufferPressure;
	std::array<float, MaxParticleCount * 4> GPUBufferForce = {0};
	std::array<int, MaxParticleCount> GPUBufferType;
//	std::array<ParticleGPU,MaxParticleCount> ParticleList;
	WaterEngineGPU();
	void Init(GLFWwindow * handle);
	~WaterEngineGPU();
	void Update();
	void Render();
	void AddParticle(Particle p);
};

