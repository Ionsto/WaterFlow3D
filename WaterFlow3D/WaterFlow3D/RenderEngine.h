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
public:
	RenderEngine(GLFWwindow * handle);
	virtual ~RenderEngine();
	void Render(World & world);
	void RenderTree(ParticleTree & tree);
};

