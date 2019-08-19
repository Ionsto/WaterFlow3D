#include "RenderEngine2D.h"
#include "WaterEngine.h"
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
	glGenBuffers(1, &IndexBuffer);
	vertex_points.Init("points.vert", GL_VERTEX_SHADER);
	fragment_points.Init("points.frag", GL_FRAGMENT_SHADER);
	std::cout << "Creating program" << std::endl;
	program_points.CreateProgram();
	program_points.AddShader(vertex_points);
	program_points.AddShader(fragment_points);
	program_points.LinkProgram();
	program_points.UseProgram();
	glGenBuffers(1, &VertexBufferPoints);
	glGenBuffers(1, &DensityBufferPoints);
}


RenderEngine2D::~RenderEngine2D()
{
}

void RenderEngine2D::Render(WaterEngine* engine)
{
	RenderGrid(engine->grid);
	RenderParticles(engine->particle_list);
}
void RenderEngine2D::RenderParticles(SwapList<Particle, WaterEngine::MaxParticleCount> & list) {
	for (int i = 0; i < list.ParticleCount; ++i) {
		ParticleVertexData[i * 2] = list.GetParticle(i).Position.X;
		ParticleVertexData[i * 2 + 1] = list.GetParticle(i).Position.Y;
		ParticleDensityData[i] = (list.GetParticle(i).Stress.DY.Y + 100) / 200;
	}
	program_points.UseProgram();
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * list.ParticleCount, &ParticleVertexData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, DensityBufferPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * list.ParticleCount, &ParticleDensityData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT,GL_FALSE, 0, 0);
	glDrawArrays(GL_POINTS, 0, list.ParticleCount);
}
void RenderEngine2D::RenderGrid(VoxelGrid& grid)
{
	int itervert = 0;
	int iterindex = 0;
	int iter = 0;
	for (int x = 0; x < grid.SizeX; ++x)
	{
		for (int y = 0; y < grid.SizeY; ++y)
		{
				auto voxel = grid.Get(x, y);
				float display_value;
				switch (DebugDisplay)
				{
					case Mass:
						display_value = ((voxel.Mass)/10.0);
						break;
					case Stress:
						display_value = ((voxel.Force_Internal.Y+200)/800.0);
//						display_value = ((voxel.Pressure+2)/10.0);
						break;
					case Velocity:
						display_value = ((voxel.Velocity.Y+50)/100.0);
						//display_value = ((voxel.Force_Internal.Y+200)/800.0);
						break;
					case Force:
						display_value = ((voxel.Force.Y+100)/400.0);
						break;
					case Black:
						display_value = 0;
						break;

				}
				//display_value = voxel.Pressure;
				//display_value = -(voxel.Force.Y*10.0) + 0.5;
				//display_value = -voxel.Force.Y + 0.5;
				GridDensityData[iter++] = display_value;
				GridVertexData[itervert++] = x;
				GridVertexData[itervert++] = y;
				if (y + 1 < grid.SizeY && x + 1 < grid.SizeX) {
					GridIndexData[iterindex++] = (y)+((x)* VoxelGrid::SizeY);
					GridIndexData[iterindex++] = (y + 1) + ((x)* VoxelGrid::SizeY);
					GridIndexData[iterindex++] = (y + 1) + ((x + 1) * VoxelGrid::SizeY);
					GridIndexData[iterindex++] = (y)+((x + 1) * VoxelGrid::SizeY);
				}
//				GridDensityData[iter++] = display_value;
//				GridVertexData[itervert++] = x;
//				GridVertexData[itervert++] = y + 1;
//				GridDensityData[iter++] = display_value;
//				GridVertexData[itervert++] = x + 1;
//				GridVertexData[itervert++] = y + 1;
//				GridDensityData[iter++] = display_value;
//				GridVertexData[itervert++] = x + 1;
//				GridVertexData[itervert++] = y;
		}
	}
	
	program_grid.UseProgram();
	int VertexCount = iter;
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GridVertexData.size() , &GridVertexData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, DensityBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GridDensityData.size() , &GridDensityData[0], GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT,GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GridIndexData.size() * sizeof(unsigned int), &GridIndexData[0], GL_STREAM_DRAW);
	glDrawElements(
		GL_QUADS,      // mode
		GridIndexData.size(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);
	//glDrawArrays(GL_QUADS, 0, VertexCount);
}
