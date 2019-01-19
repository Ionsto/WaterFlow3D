#include "GameManager.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
GameManager::GameManager()
{
	if (!glfwInit())
	{
		// Initialization failed
		throw;
	}
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_VERSION_MINOR, 2);;
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	std::cout << "Loading window" << std::endl;
	Window_Handle = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!Window_Handle)
	{
		std::cout << "Window load failed" << std::endl;
		throw;
	}
	glfwSetFramebufferSizeCallback(Window_Handle, framebuffer_size_callback);
	glfwMakeContextCurrent(Window_Handle);
	glfwSwapInterval(0);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW! I'm out!" << std::endl;
		throw;
	}
	renderengine = std::make_unique<RenderEngine>(Window_Handle);
	world = std::make_unique<World>();
	int width, height;
	glfwGetFramebufferSize(Window_Handle, &width, &height);
	glViewport(0, 0, width, height);
}


GameManager::~GameManager()
{
	glfwDestroyWindow(Window_Handle);
	glfwTerminate();
}

void GameManager::Run()
{
	DtCounter = std::chrono::high_resolution_clock::now();
	while (Running)
	{
		Update();
		Render();
		glfwSwapBuffers(Window_Handle);
		glfwPollEvents();
		if (FrameCount++ == 100)
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto dt = now - StartTime;
			StartTime = now;
			std::cout << "Frametime:" << (dt.count() / (100.0 * 1000000000)) << "\n fps:" << ((100.0*1000000000) / dt.count()) << "\n";
			FrameCount = 0;
		}
		auto end = std::chrono::high_resolution_clock::now();
		DeltaTime = (end - DtCounter).count()/1000000000.0;
		DtCounter = end;
	}
}

void GameManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	renderengine->Render(*world.get());
}
void GameManager::PollInput()
{
	if (glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (clickticker++ == 20) {
			double xpos, ypos;
			glfwGetCursorPos(Window_Handle, &xpos, &ypos);
			int width, height;
			int left, top, right, bottom;
			glfwGetWindowFrameSize(Window_Handle, &left, &top, &right, &bottom);
			glfwGetWindowSize(Window_Handle, &width, &height);
			float x = (xpos / width) * world->waterengine.ParticleList.TotalWidth;
			float y = (1.0 - (ypos / height)) * world->waterengine.ParticleList.TotalWidth;
			Vector pos(x, y, 0);
			pos += Vector(((rand() % 100) / 100.0f) - 0.5) * 10;
			if (glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				world->AddSand(pos);
			}
			else
			{
				world->AddWater(pos);
			}
			clickticker = 0;
		}
	}
}
void GameManager::Update()
{
	PollInput();
	world->Update(DeltaTime);
	if (glfwWindowShouldClose(Window_Handle)) {
		Running = false;
	}
}