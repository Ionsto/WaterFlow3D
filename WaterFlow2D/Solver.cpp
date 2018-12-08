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
		for(int j = 0;j < ParticleCount;++j)
		{
			auto & ParticleJ = GetParticle(j); 
			if(i != j)
			{
				Vector difference = ParticleI.Position - ParticleJ.Position; 
				float DistanceSquared = difference.Dot(difference);
			}
		}
	}
}
void Solver::UpdateConditions()
{
}
void Solver::Intergrate()
{
	for(int i = 0;i < ParticleCount;++i)
	{
	//	auto Force =
		auto & ParticleI = GetParticle(i); 
		Vector Acceleration = (ParticleI.Force / ParticleI.Mass) * Solver::DeltaTime;
		ParticleI.Position = (ParticleI.Position * 2) - ParticleI.PositionOld + Acceleration;
				
	}
}

