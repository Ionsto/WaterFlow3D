#include "Solver.h"
#include <iostream>
Solver::Solver()
{
	static const constexpr float Size = 50;
	static const constexpr int Rank = 4;
	static const constexpr float Mass = 10;
	for(float x = 0; x <= Size; x += BoundaryDensity) 
	{
		for(int r = 0; r < Rank;r++)
		{
			AddParticle(Vector(x,-BoundaryDensity * r),Mass);
			AddParticle(Vector(x,50 + (BoundaryDensity * r)),Mass);
			AddParticle(Vector(-BoundaryDensity * r,x),Mass);
			AddParticle(Vector(50 + (BoundaryDensity * r),x),Mass);
		}
		DummyParticles += Rank * 4;
	}
}
void Solver::Update()
{
	//UpdateConditions();
	UpdatePressure();
	UpdateForces();
	Intergrate();
}
float Solver::Poly6(float r)
{
	return 315.f * std::pow((SmoothingParam * SmoothingParam) - (r * r), 3.0) / (64.f * 3.14 * pow(SmoothingParam, 9.0f));
}
float Solver::SpikyGrad(float r)
{
	return  -45.f / (3.14 * std::pow(SmoothingParam, 6.0)) * std::pow(SmoothingParam - r, 2.0);
}
float Solver::LaplaceVisc(float r)
{
	return 45.f / (3.14 * pow(SmoothingParam, 6.0)) * (SmoothingParam - r);
}

void Solver::UpdatePressure()
{
#pragma omp parallel for  
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
				float Distance = difference.Magnitude();
				if(Distance < SmoothingParam)
				{
					ParticleI.Density += ParticleJ.Mass * Poly6(Distance); 
				}
			}
		}
		//ParticleI.Pressure = std::max(0.0f,(ParticleI.Density - Density0) * GasConstant);
		ParticleI.Pressure = ((ParticleI.Density - Density0) * GasConstant);
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
	for(int i = 0;i < ParticleCount-1;++i)
	{
		auto & ParticleI = GetParticle(i); 
		for(int j = i+1;j < ParticleCount;++j)
		{
			auto & ParticleJ = GetParticle(j); 
			Vector difference = ParticleI.Position - ParticleJ.Position;
			float distance = difference.Magnitude();
			if(distance != 0 && distance < SmoothingParam && ParticleJ.Density != 0 && ParticleI.Density != 0)
			{
				float PressureForce = ((ParticleI.Pressure + ParticleJ.Pressure)) * SpikyGrad(distance);
				difference = difference / distance;
				ParticleI.Force -= difference * (PressureForce * ParticleJ.Mass /(2.0*ParticleJ.Density));  
				ParticleJ.Force += difference * (PressureForce * ParticleI.Mass /(2.0*ParticleI.Density));  
				Vector Visc = ((ParticleJ.Position - ParticleJ.PositionOld) - (ParticleI.Position - ParticleI.PositionOld)) * LaplaceVisc(distance) * Viscosity;
				ParticleI.Force += Visc /ParticleI.Mass * ParticleJ.Density;
				ParticleJ.Force -= Visc / ParticleJ.Mass * ParticleI.Density;
			}
		}
		ParticleI.Force.Y -= ParticleI.Mass * Gravity;
	}
}
void Solver::Print(int t,std::ofstream & out)
{
	for(int i = 0;i < ParticleCount;++i)
	{
		auto & ParticleI = GetParticle(i); 
		out<<t<<","<<ParticleI.Position.X << "," <<ParticleI.Position.Y<<","<<ParticleI.Density<<"\n";
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
			Particle.PositionOld.X = Particle.Position.X + (Particle.PositionOld.X * Restitution); 
			std::swap(Particle.Position.X,Particle.PositionOld.X);
		}
		if(Particle.Position.Y > 50)
		{
			Particle.PositionOld.Y -= Particle.Position.Y;
			Particle.Position.Y = 50;
			Particle.PositionOld.Y = Particle.Position.Y + (Particle.PositionOld.Y * Restitution);
			std::swap(Particle.Position.Y,Particle.PositionOld.Y);
		}
	}
}

void Solver::Intergrate()
{
	for(int i = DummyParticles;i < ParticleCount;++i)
	{
		auto & ParticleI = GetParticle(i); 
		Vector Acceleration = (ParticleI.Force / ParticleI.Mass) * Solver::DeltaTime* Solver::DeltaTime;;
		Vector old = ParticleI.Position;
		ParticleI.Position = (ParticleI.Position * (2 - Damping)) - (ParticleI.PositionOld * (1-Damping)) + Acceleration;
		ParticleI.PositionOld = old;
		ParticleI.Force = Vector();
	}
}
void Solver::AddParticle(Vector vec,float mass)
{
	if(ParticleCount < MaxParticles)
	{
		ParticleSwapList[ParticleCount] = Particle();
		ParticleSwapList[ParticleCount].Position = vec;
		ParticleSwapList[ParticleCount].PositionOld = vec;
		ParticleSwapList[ParticleCount].Mass = mass;
	//	ParticleSwapList[ParticleCount].PositionOld.Y += 0.1;
		ParticleCount++;
	}
}
