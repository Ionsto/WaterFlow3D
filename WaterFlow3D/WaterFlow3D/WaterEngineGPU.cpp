#include "WaterEngineGPU.h"
#include <iostream>
#include <chrono>


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
	Compute_Intergrate.Init("sim_intergrate.comp",GL_COMPUTE_SHADER);
	Program_Intergrate.CreateProgram();
	Program_Intergrate.AddShader(Compute_Intergrate);
	Program_Intergrate.LinkProgram();
	Compute_Density.Init("sim_density.comp",GL_COMPUTE_SHADER);
	Program_Density.CreateProgram();
	Program_Density.AddShader(Compute_Density);
	Program_Density.LinkProgram();
	Compute_Force.Init("sim_force.comp",GL_COMPUTE_SHADER);
	Program_Force.CreateProgram();
	Program_Force.AddShader(Compute_Force);
	Program_Force.LinkProgram();
	Compute_Flow.Init("sim_flow.comp", GL_COMPUTE_SHADER);
	Program_Flow.CreateProgram();
	Program_Flow.AddShader(Compute_Flow);
	Program_Flow.LinkProgram();
	glGenBuffers(1, &PositionBuffer);
	glGenBuffers(1, &PositionOldBuffer);
	glGenBuffers(1, &ForceBuffer);
	glGenBuffers(1, &PressureBuffer);
	glGenBuffers(1, &DensityBuffer);
	glGenBuffers(1, &TypeBuffer);
	glGenBuffers(1, &SceneBuffer);

	for (int i = 0; i < 1000; ++i) {
		ParticleGPU p;
		p.Position = Vector(rand() % 5000,rand() % 5000,rand() % 5000) / 50;
		p.PositionOld = p.Position;
		p.Type = 1;
		ParticleList.AddParticle(p);
	}
	ParticleCount = ParticleList.ParticleCount;
	GPUscene.ParticleCount = ParticleCount;
	for (int i = 0, inc = 0; i < ParticleCount; ++i) {
			GPUBufferPosition[inc] = ParticleList.GetParticle(i).Position.X;
			GPUBufferPositionOld[inc++] = ParticleList.GetParticle(i).PositionOld.X;
			GPUBufferPosition[inc] = ParticleList.GetParticle(i).Position.Y;
			GPUBufferPositionOld[inc++] = ParticleList.GetParticle(i).PositionOld.Y;
			GPUBufferPosition[inc] = ParticleList.GetParticle(i).Position.Z;
			GPUBufferPositionOld[inc++] = ParticleList.GetParticle(i).PositionOld.Z;
			GPUBufferType[i] = ParticleList.GetParticle(i).Type;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount * 4, GPUBufferPosition.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PositionOldBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount * 4, GPUBufferPositionOld.data() , GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, DensityBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount, GPUBufferDensity.data() , GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, PressureBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * MaxParticleCount, GPUBufferPressure.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ForceBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) *  MaxParticleCount * 4, GPUBufferForce.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, TypeBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(int) * MaxParticleCount, GPUBufferType.data(), GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SceneBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(GPUScene),&GPUscene, GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
	FlowCount = -1000;
}


WaterEngineGPU::~WaterEngineGPU()
{
}

void WaterEngineGPU::Update()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, PositionBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, PositionOldBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ForceBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, DensityBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, PressureBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, TypeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, SceneBuffer);
	GPUscene.ParticleCount = ParticleCount;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SceneBuffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GPUScene), &GPUscene);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	if(FlowCount++ > 10)
		{
			Program_Flow.UseProgram();
			glDispatchCompute(1, (GLuint)1, 1);
			FlowCount = 0;
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SceneBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GPUScene), &GPUscene);
	ParticleCount = GPUscene.ParticleCount;
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	if (ParticleCount > 0)
	{
		auto start = std::chrono::high_resolution_clock::now();
		{
			Program_Density.UseProgram();
			glUniform1i(0, ParticleCount);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, PositionBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, PositionOldBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, PressureBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, DensityBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, TypeBuffer);
			glDispatchCompute((ParticleCount+127)/128, (GLuint)1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
		{
			Program_Force.UseProgram();
			glUniform1i(0, ParticleCount);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, PositionBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, PositionOldBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ForceBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, DensityBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, PressureBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, TypeBuffer);
			glDispatchCompute((ParticleCount+127)/128, (GLuint)1, 1);
		}
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		{
			Program_Intergrate.UseProgram();
			glUniform1i(0, ParticleCount);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, PositionBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, PositionOldBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ForceBuffer);
			//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, TypeBuffer);
			glDispatchCompute((ParticleCount + 127)/128, (GLuint)1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto dt = end - start;
		//std::cout << "sim time:" << (dt.count() / (1000000.0)) << "\n";
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
	pGPU.Type = p.Type;
	ParticleList.AddParticle(pGPU);
}