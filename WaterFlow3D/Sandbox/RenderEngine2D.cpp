#include "RenderEngine2D.h"
#include <iostream>
#include <GL/GL.h>
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



RenderEngine2D::RenderEngine2D(GLFWwindow * window)
{
	Window = window;
	glDebugMessageCallback(MessageCallback_GPU, 0);
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	std::cout<< "Renderer supported:" << renderer << std::endl;
	std::cout<< "OpenGL version supported:" << version << std::endl;
	glPointSize(2);
	std::cout<< "Init shaders" << renderer << std::endl;
	vertex.Init("quad.vert", GL_VERTEX_SHADER);
	fragment.Init("points.frag", GL_FRAGMENT_SHADER);
	std::cout << "Creating program" << std::endl;
	program_grid.CreateProgram();
	program_grid.AddShader(vertex);
	program_grid.AddShader(fragment);
	program_grid.LinkProgram();
	program_grid.UseProgram();
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &DensityBuffer);
}


RenderEngine2D::~RenderEngine2D()
{
}

void RenderEngine2D::Render(VoxelGrid& grid)
{
	static constexpr int MaxVerts = grid.SizeX * grid.SizeY * 4;
	std::array<float,2 * MaxVerts> VertexData;
	std::array<float,MaxVerts> DensityData;
	int itervert = 0;
	int iter = 0;
	for (int x = 1; x < grid.SizeX-1; ++x)
	{
		for (int y = 1; y < grid.SizeY-1; ++y)
		{
			if (iter <= MaxVerts)
			{
				auto voxel = grid.Get(x, y);
				float display_value;
				switch (DebugDisplay)
				{
					case Density:
						display_value = voxel.Mass;
						break;
					case Pressure:
//						display_value = ((voxel.Pressure+2)/10.0);
						break;
					case Velocity:
						display_value = ((voxel.Velocity.Y+100)/400.0);
						break;
					case Force:
						display_value = ((voxel.Force.Y+100)/400.0);
						break;
				}
				//display_value = voxel.Pressure;
				//display_value = -(voxel.Force.Y*10.0) + 0.5;
				//display_value = -voxel.Force.Y + 0.5;
				DensityData[iter++] = display_value;
				VertexData[itervert++] = x;
				VertexData[itervert++] = y;
				DensityData[iter++] = display_value;
				VertexData[itervert++] = x;
				VertexData[itervert++] = y + 1;
				DensityData[iter++] = display_value;
				VertexData[itervert++] = x + 1;
				VertexData[itervert++] = y + 1;
				DensityData[iter++] = display_value;
				VertexData[itervert++] = x + 1;
				VertexData[itervert++] = y;
			}
		}
	}
	
	program_grid.UseProgram();
	int VertexCount = iter;
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * MaxVerts, &VertexData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, DensityBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MaxVerts, &DensityData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT,GL_FALSE, 0, 0);
	glDrawArrays(GL_QUADS, 0, VertexCount);

}
