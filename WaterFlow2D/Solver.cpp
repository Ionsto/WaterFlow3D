#include "Solver.h"
#include <iostream>
void Solver::Update()
{
	UpdateConditions();
	UpdatePressure();
	UpdateForces();
	Intergrate();
}

void Solver::UpdatePressure()
{
	for(int i = 0;i < ParticleCount;++i)
	{
		auto & ParticleI = GetParticle(i); 
		ParticleI.Density = 0;
		for(int j = 0;j < ParticleCount;++j)
		{
			auto & ParticleJ = GetParticle(j); 
			if(i != j)
			{
				Vector difference = ParticleI.Position - ParticleJ.Position;
				ParticleI.Density += ParticleJ.Mass * SmoothingKernal(difference.Magnitude()); 
			}
		}
		ParticleI.Pressure = (ParticleI.Density - Density0) * GasConstant;
	}
}
float Solver::SmoothingKernal(float Radius)
{
	float x = Radius/SmoothingParam;
	const constexpr float factor = 1/(3.14*SmoothingParam*SmoothingParam*SmoothingParam);
	if(x < 1)
	{
		return factor * (1 - (1.5 * x * x) + (0.75 * x * x * x));
	}
	else if(x < 2)
	{
		return factor * (0.25 * (2 - x) *  (2 - x) *  (2 - x)); 
	}
	else
	{
		return 0;
	}
}
float Solver::SmoothingKernalGradient(float Radius)
{
	float x = Radius/SmoothingParam;
	const constexpr float factor = 1/(3.14*SmoothingParam*SmoothingParam*SmoothingParam*SmoothingParam);
	if(x < 1)
	{
		return factor * ((9/4 * x * x) + (3 * x));
	}
	else if(x < 2)
	{
		return factor * (-0.75 * (2 - x) *  (2 - x)); 
	}
	else
	{
		return 0;
	}
}
void Solver::UpdateForces()
{
	for(int i = 0;i < ParticleCount;++i)
	{
		auto & ParticleI = GetParticle(i); 
		for(int j = i+1;j < ParticleCount;++j)
		{
			auto & ParticleJ = GetParticle(j); 
			Vector difference = ParticleI.Position - ParticleJ.Position;
			float distance = difference.Magnitude();
			if(distance != 0)
			{
				float PressureForce = ((ParticleI.Pressure + ParticleJ.Pressure)/distance) * SmoothingKernalGradient(distance);
				ParticleI.Force += difference * (PressureForce * ParticleJ.Mass /(2*ParticleJ.Pressure));  
				ParticleJ.Force -= difference * (PressureForce * ParticleI.Mass /(2*ParticleI.Pressure));  
			}
		}
		ParticleI.Force.Y -= ParticleI.Mass * Gravity;
	}
}
void Solver::Print(int t)
{
	for(int i = 0;i < ParticleCount;++i)
	{
		auto & ParticleI = GetParticle(i); 
		std::cout<<t<<","<<ParticleI.Position.X << "," <<ParticleI.Position.Y<<std::endl;
	}

}

void Solver::UpdateConditions()
{
	for(int i = 0;i < ParticleCount;++i)
	{
		auto & Particle = GetParticle(i); 
		if(Particle.Position.X < 0)
		{
			Particle.PositionOld.X -= Particle.Position.X;
			Particle.Position.X = 0;
			Particle.PositionOld.X += Restitution * (Particle.Position.X - Particle.PositionOld.X); 
			std::swap(Particle.Position.X,Particle.PositionOld.X);
		}
		if(Particle.Position.Y < 0)
		{
			Particle.PositionOld.Y -= Particle.Position.Y;
			Particle.Position.Y = 0;
			Particle.PositionOld.Y += Restitution * (Particle.Position.Y - Particle.PositionOld.Y); 
			std::swap(Particle.Position.Y,Particle.PositionOld.Y);
		}
		if(Particle.Position.X > 50)
		{
			Particle.PositionOld.X -= Particle.Position.X;
			Particle.Position.X = 50;
			Particle.PositionOld.X -= Restitution * (Particle.Position.X - Particle.PositionOld.X); 
			std::swap(Particle.Position.X,Particle.PositionOld.X);
		}
		if(Particle.Position.Y > 50)
		{
			Particle.PositionOld.Y -= Particle.Position.Y;
			Particle.Position.Y = 50;
			Particle.PositionOld.Y -= Restitution * (Particle.Position.Y - Particle.PositionOld.Y); 
			std::swap(Particle.Position.Y,Particle.PositionOld.Y);
		}
	}
}

void Solver::Intergrate()
{
	for(int i = 0;i < ParticleCount;++i)
	{
		auto & ParticleI = GetParticle(i); 
		Vector Acceleration = (ParticleI.Force / ParticleI.Mass) * Solver::DeltaTime* Solver::DeltaTime;;
		Vector old = ParticleI.Position;
		ParticleI.Position = (ParticleI.Position * 2) - ParticleI.PositionOld + Acceleration;
		ParticleI.PositionOld = old;
		ParticleI.Force = Vector();
	}
}
void Solver::AddParticle(Vector vec)
{
	if(ParticleCount < MaxParticles)
	{
		ParticleSwapList[ParticleCount] = Particle();
		ParticleSwapList[ParticleCount].Position = vec;
		ParticleSwapList[ParticleCount].PositionOld = vec;
		ParticleCount++;
	}
}
