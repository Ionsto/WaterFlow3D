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
	std::cout << "Loading window" << std::endl;
	Window_Handle = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!Window_Handle)
	{
		std::cout << "Window load failed" << std::endl;
		throw;
	}
	glfwSetFramebufferSizeCallback(Window_Handle, framebuffer_size_callback);
	glfwMakeContextCurrent(Window_Handle);
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
	while (Running)
	{
		Update();
		Render();
		glfwSwapBuffers(Window_Handle);
		glfwPollEvents();
	}
}

void GameManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	renderengine->Render(*world.get());
}
void GameManager::Update()
{
	if (glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (clickticker++ == 10) {
			double xpos, ypos;
			glfwGetCursorPos(Window_Handle, &xpos, &ypos);
			int width, height;
			glfwGetFramebufferSize(Window_Handle, &width, &height);
			float x = (xpos / width) * world->waterengine.ParticleList.Width;
			float y = (1-(ypos / width)) * world->waterengine.ParticleList.Width;
			Vector pos(x, y, 0);
			Particle p;
			p.Mass = 1000;
			p.Viscosity = 0;
			p.Density0 = 500;
			p.Position = pos;
			p.PositionOld = pos;
			world->waterengine.AddParticle(p);
			clickticker = 0;
		}
	}
	world->Update();
	if (glfwWindowShouldClose(Window_Handle)) {
		Running = false;
	}
}