#include "RenderEngineGPU.h"

void GLAPIENTRY
MessageCallback_GPU(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}



RenderEngineGPU::RenderEngineGPU(GLFWwindow * handle,World & world)
{
	std::cout<< "Debug messages" <<  std::endl;
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback_GPU, 0);
	Window_Handle = handle;
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	std::cout<< "Renderer supported:" << renderer << std::endl;
	std::cout<< "OpenGL version supported:" << version << std::endl;
	glPointSize(2);
	std::cout<< "Init shaders" << renderer << std::endl;
	vertex.Init("points.vert", GL_VERTEX_SHADER);
	fragment.Init("points.frag", GL_FRAGMENT_SHADER);
	std::cout << "Creating program" << std::endl;
	program.CreateProgram();
	program.AddShader(vertex);
	program.AddShader(fragment);
	program.LinkProgram();
	program.UseProgram();
	GLint computesize = 0;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &computesize);
	std::cout << "Largest compute group" << computesize << "\n";
	glGenVertexArrays(2, VertexArrays);
	glGenBuffers(2, VertexBuffers);
	glGenBuffers(2, TypeBuffers);
	glBindVertexArray(VertexArrays[0]);
	glBindBuffer(GL_ARRAY_BUFFER, world.waterengine.PositionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 * 4, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	//GLint loc = glGetUniformLocation(program.GetProgram(), "uColor");
	//glDisable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE,GL_DONT_CARE,GL_DEBUG_SEVERITY_MEDIUM,0,0,false);
	glDebugMessageControl(GL_DONT_CARE,GL_DONT_CARE,GL_DEBUG_SEVERITY_LOW,0,0,false);
	glDebugMessageControl(GL_DONT_CARE,GL_DONT_CARE,GL_DEBUG_SEVERITY_NOTIFICATION,0,0,false);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}


RenderEngineGPU::~RenderEngineGPU()
{
}

void RenderEngineGPU::Render(World & world)
{

	program.UseProgram();
	glBindVertexArray(VertexArrays[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * MaxRenderParticles, &VertexData[0], GL_STREAM_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, TypeBuffers[CurrentBuffer]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MaxRenderParticles, &TypeData[0], GL_STREAM_DRAW);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
	
	glDrawArrays(GL_POINTS, 0, world.waterengine.MaxParticleCount);
	glBindVertexArray(0);
	/*glBindBuffer(GL_ARRAY_BUFFER, TypeBuffers[CurrentBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * TypeData.size(), &TypeData.front(), GL_STREAM_DRAW);
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, 0, 0);
	*/
	//glEnableVertexAttribArray(1);

	//glBindVertexArray(VertexArrays[CurrentBuffer]);
	//glDrawArrays(GL_POINTS, 0, 1);
	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//CurrentBuffer ^= 1;
	return;
}