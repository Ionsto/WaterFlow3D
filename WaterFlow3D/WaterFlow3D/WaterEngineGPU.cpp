#include "WaterEngineGPU.h"
#include <iostream>



WaterEngineGPU::WaterEngineGPU()
{
}
void WaterEngineGPU::Init(GLFWwindow * handle)
{
	Window_Handle = handle;
	ComputeTest.Init("test.comp", GL_COMPUTE_SHADER);
	TestProgram.CreateProgram();
	TestProgram.AddShader(ComputeTest);
	TestProgram.LinkProgram();
	TestProgram.UseProgram();
	glGenBuffers(1, &PositionBuffer);
	glGenBuffers(1, &PositionOldBuffer);
	glGenBuffers(1, &ForceBuffer);
	glGenBuffers(1, &PressureBuffer);
	glGenBuffers(1, &DensityBuffer);
	glGenBuffers(1, &TypeBuffer);
	ParticleCount = 0;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount * 3, GPUBufferPosition.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionOldBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount * 3, GPUBufferPositionOld.data() , GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, DensityBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount, GPUBufferDensity.data() , GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PressureBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount, GPUBufferPressure.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ForceBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 3 * MaxParticleCount, GPUBufferForce.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 3 * MaxParticleCount, GPUBufferForce.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
}


WaterEngineGPU::~WaterEngineGPU()
{
}

void WaterEngineGPU::Update()
{
	ParticleCount = ParticleList.ParticleCount;
	if (ParticleCount > 0)
	{
		//std::cout << "Particle Count:" << ParticleCount << "\n";
		/*for (int i = 0, inc = 0; i < ParticleCount; ++i) {
			GPUBufferDensity[i] = ParticleList.GetParticle(i).Density;
			GPUBufferPressure[i] = ParticleList.GetParticle(i).Pressure;
			GPUBufferPosition[inc] = ParticleList.GetParticle(i).Position.X;
			GPUBufferForce[inc] = ParticleList.GetParticle(i).Force.X;
			GPUBufferPositionOld[inc++] = ParticleList.GetParticle(i).PositionOld.X;
			GPUBufferPosition[inc] = ParticleList.GetParticle(i).Position.Y;
			GPUBufferForce[inc] = ParticleList.GetParticle(i).Force.Y;
			GPUBufferPositionOld[inc++] = ParticleList.GetParticle(i).PositionOld.Y;
			GPUBufferPosition[inc] = ParticleList.GetParticle(i).Position.Z;
			GPUBufferForce[inc] = ParticleList.GetParticle(i).Force.Z;
			GPUBufferPositionOld[inc++] = ParticleList.GetParticle(i).PositionOld.Z;
		}
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionBuffer);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * 3 * ParticleCount, GPUBufferPosition.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionOldBuffer);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * 3 * ParticleCount, GPUBufferPositionOld.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ForceBuffer);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * 3 * ParticleCount, GPUBufferForce.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, PressureBuffer);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * ParticleCount, GPUBufferPressure.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, DensityBuffer);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * ParticleCount, GPUBufferDensity.data());
		*/
		{  
			TestProgram.UseProgram();
			glUniform1i(0, ParticleCount);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, PositionBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, PositionOldBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ForceBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, PressureBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, DensityBuffer);
			glDispatchCompute(ParticleCount, (GLuint)1, 1);
		}
	/*
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, ParticleCount * 3 * sizeof(float), GPUBufferPosition.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionOldBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, ParticleCount * 3 * sizeof(float), GPUBufferPositionOld.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ForceBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, ParticleCount * 3 * sizeof(float), GPUBufferForce.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, DensityBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, ParticleCount * sizeof(float), GPUBufferDensity.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PressureBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, ParticleCount * sizeof(float), GPUBufferPressure.data());
	for (int i = 0, inc = 0; i < ParticleCount; ++i) {
		ParticleList.GetParticle(i).Density = GPUBufferDensity[i];
		ParticleList.GetParticle(i).Pressure = GPUBufferPressure[i];
		ParticleList.GetParticle(i).PositionOld.X = GPUBufferPositionOld[inc];
		ParticleList.GetParticle(i).Force.X = GPUBufferForce[inc];
		ParticleList.GetParticle(i).Position.X = GPUBufferPosition[inc++];
		ParticleList.GetParticle(i).PositionOld.Y = GPUBufferPositionOld[inc];
		ParticleList.GetParticle(i).Force.Y = GPUBufferForce[inc];
		ParticleList.GetParticle(i).Position.Y = GPUBufferPosition[inc++];
		ParticleList.GetParticle(i).PositionOld.Z = GPUBufferPositionOld[inc];
		ParticleList.GetParticle(i).Force.Z = GPUBufferForce[inc];
		ParticleList.GetParticle(i).Position.Z = GPUBufferPosition[inc++];
	}
	*/
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0 );
	}
}
void WaterEngineGPU::Render()
{
	//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}
void WaterEngineGPU::AddParticle(Particle p)
{
	ParticleGPU pGPU;
	pGPU.Position = p.Position;
	pGPU.PositionOld = p.PositionOld;
	ParticleList.AddParticle(pGPU);
}