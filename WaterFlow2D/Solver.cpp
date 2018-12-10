#include "Solver.h"

void Solver::Update()
{
	UpdateConditions();
	UpdatePressure();
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
				ParticleI.Density += ParticleJ.Mass; 
			}
		}
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


void Solver::UpdateConditions()
{
}

void Solver::Intergrate()
{
	for(int i = 0;i < ParticleCount;++i)
	{
		auto & ParticleI = GetParticle(i); 
		Vector Acceleration = (ParticleI.Force / ParticleI.Mass) * Solver::DeltaTime;
		ParticleI.Position = (ParticleI.Position * 2) - ParticleI.PositionOld + Acceleration;
				
	}
}

