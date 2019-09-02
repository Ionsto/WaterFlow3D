#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "WaterEngine.h"
#include "VoxelGrid.h"
#include "SwapList.h"
class RenderEngine2D
{
private:
	GLFWwindow* Window;
	ShaderProgram program_grid;
	Shader vertex;
	Shader fragment;
	ShaderProgram program_points;
	Shader vertex_points;
	Shader fragment_points;
	GLuint VertexBuffer;
	GLuint DensityBuffer;
	GLuint IndexBuffer;
	GLuint VertexBufferPoints;
	GLuint DensityBufferPoints;
public:
	static constexpr int MaxVerts = VoxelGrid::SizeX * VoxelGrid::SizeY;
	std::array<float,2 * MaxVerts> GridVertexData;
	std::array<float,MaxVerts> GridDensityData;
	std::array<unsigned int, (VoxelGrid::SizeX - 1) * (VoxelGrid::SizeY-1) * 6> GridIndexData;
	static constexpr int MaxParticles = WaterEngine::MaxParticleCount;
	std::array<float, 2 * MaxParticles> ParticleVertexData;
	std::array<float, MaxParticles> ParticleDensityData;
	enum DisplayType {
		Mass,
		Stress,
		Velocity,
		Force,
		Black
	} DebugDisplay = Mass;
	RenderEngine2D(GLFWwindow* Window);
	~RenderEngine2D();
	void Render(WaterEngine* engine);
	void RenderParticles(SwapList<Particle,WaterEngine::MaxParticleCount>& list);
	void RenderGrid(VoxelGrid& grid);
};

