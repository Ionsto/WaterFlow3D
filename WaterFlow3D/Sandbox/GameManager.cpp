#include "GameManager.h"
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
GameManager::GameManager()
{
	std::cout << "Attempting to init glfw" << std::endl;
	if (!glfwInit())
	{
		std::cout << "Couldn't init glfw" << std::endl;
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
	std::cout << "Setting up opengl" << std::endl;
	glfwSetFramebufferSizeCallback(Window_Handle, framebuffer_size_callback);
	glfwMakeContextCurrent(Window_Handle);
	glfwSwapInterval(0);
	std::cout << "Init glew" << std::endl;
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW! I'm out!" << std::endl;
		throw;
	}
	std::cout << "Init world" << std::endl;
	waterengine = std::make_unique<WaterEngine>();
	std::cout << "Init GPU sys" << std::endl;
//	world->waterengine.Init(Window_Handle);
	//world->AddWater(Vector(100,100));
	std::cout << "Init render engine" << std::endl;
	renderengine = std::make_unique<RenderEngine2D>(Window_Handle);
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
	std::cout << "Starting game" << std::endl;
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
			std::cout << "Frametime:" << (dt.count() / (100.0 * 1000000000)) << "\n fps:" << ((100.0 * 1000000000) / dt.count()) << "\n" << " particles: "<< waterengine->particle_list.ParticleCount<<"\n";
			//std::cout << "Particle count:" << world->waterengine.ParticleCount << "\n";
			FrameCount = 0;
		}
		auto end = std::chrono::high_resolution_clock::now();
		DeltaTime = (end - DtCounter).count() / 1000000000.0;
		DtCounter = end;
	}
}

void GameManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderengine->Render(waterengine.get());
	/*renderengine->RotateCounter += renderengine->DRotate * DeltaTime;
	if (renderengine->RotateCounter > 360) {
		renderengine->RotateCounter -= 360;
	}*/
	//world->waterengine.Render();
}
void GameManager::PollInput()
{
	if (glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (clickticker >= 0.001) {
			double xpos, ypos;
			glfwGetCursorPos(Window_Handle, &xpos, &ypos);
			int width, height;
			int left, top, right, bottom;
			glfwGetWindowFrameSize(Window_Handle, &left, &top, &right, &bottom);
			glfwGetWindowSize(Window_Handle, &width, &height);
			float x = (xpos / width) * waterengine->grid.SizeX;
			float y = (1.0 - (ypos / height)) * waterengine->grid.SizeY;
			Vector pos(x, y, 0);
			pos.X += (((rand() % 100) / 100.0f) - 0.5) * 1;
			pos.Y += (((rand() % 100) / 100.0f) - 0.5) * 1;
			if (glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
//				world->AddSand(pos);
				waterengine->AddWater(pos);
			}
			else
			{
//				world->AddWater(pos);
			}
			clickticker = 0;
		}
		clickticker += DeltaTime;
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_R)){
		for (int i = waterengine->particle_list.ParticleCount - 1;i != -1; --i) {
			waterengine->particle_list.RemoveParticle(i);
		}
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_1)){
		renderengine->DebugDisplay = RenderEngine2D::Mass;
		glfwSetWindowTitle(Window_Handle,"Mass");
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_2)){
		renderengine->DebugDisplay = RenderEngine2D::Stress;
		glfwSetWindowTitle(Window_Handle,"Stress");
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_3)){
		renderengine->DebugDisplay = RenderEngine2D::Velocity;
		glfwSetWindowTitle(Window_Handle,"Velocity");
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_4)){
		renderengine->DebugDisplay = RenderEngine2D::Force;
		glfwSetWindowTitle(Window_Handle,"Force");
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_5)){
		renderengine->DebugDisplay = RenderEngine2D::Black;
		glfwSetWindowTitle(Window_Handle,"");
	}

}
void GameManager::Update()
{
	PollInput();
	this->waterengine->Update(DeltaTime);
//	world->Update(DeltaTime);
	if (glfwWindowShouldClose(Window_Handle)) {
		Running = false;
	}
}