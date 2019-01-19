#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <array>
#include "World.h"
#include <memory>
#include "Shader.h"
#include "ParticleTree.h"
#include "ShaderProgram.h"
class RenderEngine
{
private:
	ShaderProgram program;
	Shader vertex;
	Shader fragment;
	GLFWwindow* Window_Handle;
	int CurrentBuffer = 0;
	GLuint VertexArrays[2];
	GLuint TestVertexArray;
	GLuint TestVertexBuffer;
	int PointCount[2] = { 0,0 };
	GLuint VertexBuffers[2];
	GLuint TypeBuffers[2];
	static const constexpr int MaxRenderParticles = 10000;
public:
	RenderEngine(GLFWwindow * handle);
	virtual ~RenderEngine();
	void Render(World & world);
	void RenderTree(ParticleTree & tree);
	void RenderTreeLegacy(ParticleTree & tree);
};

