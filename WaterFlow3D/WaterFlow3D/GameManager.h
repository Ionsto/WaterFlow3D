#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "RenderEngine.h"
#include "WaterEngineParticle.h"
#include <memory>
class GameManager
{
public:
	int clickticker = 0;
	bool Running = true;
	std::unique_ptr<World> world;
	std::unique_ptr<RenderEngine> renderengine;
	GLFWwindow* Window_Handle;
	GameManager();
	~GameManager();
	void Run();
	void Update();
	void Render();
};

