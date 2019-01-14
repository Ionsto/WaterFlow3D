#include "RenderEngine.h"
#include <iostream>
#include "World.h"


RenderEngine::RenderEngine(GLFWwindow * handle)
{
	Window_Handle = handle;
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	std::cout<< "renderer supported" << renderer << std::endl;
	std::cout<< "OpenGL version supported" << version << std::endl;
	glPointSize(10);
	vertex.Init("points.vert", GL_VERTEX_SHADER);
	fragment.Init("points.frag", GL_FRAGMENT_SHADER);
	program.CreateProgram();
	program.AddShader(vertex);
	program.AddShader(fragment);
	program.LinkProgram();
	program.UseProgram();
}


RenderEngine::~RenderEngine()
{
}

void RenderEngine::Render(World & world)
{
	RenderTree(world.waterengine.ParticleList);
}


void RenderEngine::RenderTree(ParticleTree & tree)
{
	glBegin(GL_POINTS);
	glVertex3f(0.0,0.0,0);
	glVertex3f(1,1,0);
	glVertex3f(-1,-1,0);
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
					glVertex3f(particle.Position.X/tree.TotalWidth, particle.Position.Y/tree.TotalWidth, particle.Position.Z/tree.TotalHeight);
				}
			}
		}
	}
	glEnd();
}

