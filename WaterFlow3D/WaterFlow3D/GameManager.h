#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "RenderEngineGPU.h"
#include "WaterEngineParticle.h"
#include <memory>
#include <chrono>
class GameManager
{
private:
	int FrameCount = 0;
	std::chrono::time_point<std::chrono::steady_clock> StartTime;
	std::chrono::time_point<std::chrono::steady_clock> DtCounter;
	float DeltaTime = 0;
public:
	int clickticker = 0;
	int clicktickerwater = 0;
	bool Running = true;
	std::unique_ptr<World> world;
	std::unique_ptr<RenderEngineGPU> renderengine;
	GLFWwindow* Window_Handle;
	GameManager();
	~GameManager();
	void Run();
	void Update();
	void PollInput();
	void Render();
};

