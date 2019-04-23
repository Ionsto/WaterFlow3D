#include "RenderEngine.h"
#include <iostream>
#include "World.h"


void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	/*fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
	*/
}



RenderEngine::RenderEngine(GLFWwindow * handle)
{

	std::cout<< "Debug messages" <<  std::endl;
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
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
	glGenBuffers(2, VertexBuffers);
	glGenBuffers(2, TypeBuffers);
	//GLint loc = glGetUniformLocation(program.GetProgram(), "uColor");
}
RenderEngine::~RenderEngine()
{
}

void RenderEngine::Render(World & world)
{
	//RenderTree(world.waterengine.ParticleList);
}

void RenderEngine::RenderTree(ParticleTree & tree)
{
	std::array<float,3 * MaxRenderParticles> VertexData;
	std::array<float,MaxRenderParticles> TypeData = { 0 };
	int itervert = 0;
	int iter = 0;
	for (int x = 0; x < ParticleTree::Width; ++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = tree.GetNode(x, y, z);
				for (int i = 0; i < node.ParticleCount; ++i)
				{
					auto particle = node.GetParticle(i);
					if (iter <= MaxRenderParticles)
					{
						TypeData[iter++] = (particle.Sand ? 1 : 0);
						VertexData[itervert++] = (particle.Position.X / tree.TotalWidth);
						VertexData[itervert++] = (particle.Position.Y / tree.TotalWidth);
						VertexData[itervert++] = (particle.Position.Z / tree.TotalHeight);
					}
				}
			}
		}
	}
	if (iter == MaxRenderParticles)
	{
		std::cout << "Max	Render particles hit: particles truncated";
	}
	PointCount[CurrentBuffer] = iter;
	//glBindVertexArray(TestVertexArray);
//	glDrawArrays(GL_POINTS, 0, 1);
//	glBindVertexArray(0);


	program.UseProgram();
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffers[CurrentBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * MaxRenderParticles, &VertexData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, TypeBuffers[CurrentBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MaxRenderParticles, &TypeData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT,GL_FALSE, 0, 0);
	glDrawArrays(GL_POINTS, 0,PointCount[CurrentBuffer]);
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



void RenderEngine::RenderTreeLegacy(ParticleTree & tree)
{
	glBegin(GL_POINTS);
	//glVertex3f(0.0,0.0,0);
	//glVertex3f(1,1,0);
	//glVertex3f(-1,-1,0);
	for (int x = 0; x < ParticleTree::Width; ++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = tree.GetNode(x, y, z);
				for (int i = 0; i < node.ParticleCount; ++i)
				{
					auto particle = node.GetParticle(i);
					if (particle.Sand)
					{
						//glColor3f(1, 1, 1);
					}
					else {
						//glColor3f(0, 0, 1);
					}
					glVertex3f(particle.Position.X / tree.TotalWidth, particle.Position.Y / tree.TotalWidth, particle.Position.Z / tree.TotalHeight);
				}
			}
		}
	}
	glEnd();
}

