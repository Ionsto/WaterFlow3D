#pragma once
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "RenderEngine2D.h"
#include "WaterEngine.h"
#include "VoxelGrid.h"
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
	float clickticker = 0;
	int clicktickerwater = 0;
	bool Running = true;
	std::unique_ptr<WaterEngine> waterengine;
	std::unique_ptr<RenderEngine2D> renderengine;
	GLFWwindow* Window_Handle;
	GameManager();
	~GameManager();
	void Run();
	void Update();
	void PollInput();
	void Render();
};

