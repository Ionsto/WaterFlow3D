#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "VoxelGrid.h"
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
public:
	enum DisplayType {
		Density,
		Pressure,
		Velocity,
		Force
	} DebugDisplay = Density;
	RenderEngine2D(GLFWwindow* Window);
	~RenderEngine2D();
	void Render(VoxelGrid& grid);
};

