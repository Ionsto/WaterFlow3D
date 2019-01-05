#include "WaterEngineParticle.h"
#include "Vector.h"

WaterEngineParticle::WaterEngineParticle()
{
}


WaterEngineParticle::~WaterEngineParticle()
{ 
}
void WaterEngineParticle::UpdateDensity()
{
	for(int x = 0;x < ParticleTree::Width;++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = ParticleList.GetNode(x, y, z);
				UpdateDensityNode(node, x, y, z);
			}
		}
	}
} 
void WaterEngineParticle::UpdateDensityNode(ParticleNode & node, int sx, int sy, int sz)
{
	for (int i = 0; i < node.ParticleCount; ++i)
	{
		auto & particle = node.GetParticle(i);
		particle.Density = 0;
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				for (int dz = -1; dz <= 1; ++dz)
				{
					int x = sx + dx;
					int y = sy + dy;
					int z = sz + dz;
					if (ParticleList.InBounds(x,y,z))
					{
						continue;
					}
					auto & comparenode = ParticleList.GetNode(x, y, z);
					for (int n = 0; n < comparenode.ParticleCount; ++n)
					{
						auto cparticle = comparenode.GetParticle(n);
						float Distance = (particle.Position - cparticle.Position).Magnitude();
						particle.Density += cparticle.Mass * Poly6(Distance); 
					}
				}
			}
		}
	}
}
float WaterEngineParticle::Poly6(float r)
{
	return 315.f * std::pow((SmoothingParam * SmoothingParam) - (r * r), 3.0) / (64.f * 3.14 * pow(SmoothingParam, 9.0f));
}
void WaterEngineParticle::UpdateForces()
{
	for(int x = 0;x < ParticleTree::Width;++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = ParticleList.GetNode(x, y, z);
				UpdateForcesNode(node, x, y, z);
			}
		}
	}
}
void WaterEngineParticle::UpdateForcesNode(ParticleNode & node, int sx, int sy, int sz)
{
	for (int i = 0; i < node.ParticleCount; ++i)
	{
		auto & particle = node.GetParticle(i);
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				for (int dz = -1; dz <= 1; ++dz)
				{
					int x = sx + dx;
					int y = sy + dy;
					int z = sz + dz;
					if (ParticleList.InBounds(x, y, z))
					{
						continue;
					}
					auto & comparenode = ParticleList.GetNode(x, y, z);
					for (int n = 0; n < comparenode.ParticleCount; ++n)
					{
						auto cparticle = comparenode.GetParticle(n);
						Vector difference = particle.Position - cparticle.Position;
						float distance = difference.Magnitude();
						if (distance != 0 && distance < SmoothingParam && cparticle.Density != 0 && particle.Density != 0)
						{
							float PressureForce = ((particle.Pressure + cparticle.Pressure)) * SpikyGrad(distance);
							difference = difference / distance;
							particle.Force -= difference * (PressureForce * cparticle.Mass / (2.0*cparticle.Density));
							cparticle.Force += difference * (PressureForce * particle.Mass / (2.0*particle.Density));
							//Vector Visc = ((cparticle.Position - cparticle.PositionOld) - (particle.Position - particle.PositionOld)) * LaplaceVisc(distance) * Viscosity;
							//particle.Force += Visc / particle.Mass * cparticle.Density;
							//cparticle.Force -= Visc / cparticle.Mass * particle.Density;
						}
					}
				}
			}
		}
	}
}
void WaterEngineParticle::UpdateConditions()
{

}
void WaterEngineParticle::Intergrate()
{
	for (int x = 0; x < ParticleTree::Width; ++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = ParticleList.GetNode(x, y, z);
				for (int i = 0; i < node.ParticleCount; ++i)
				{
					auto & particle = node.GetParticle(i);
					Vector Acceleration = (particle.Force / particle.Mass) * DeltaTime * DeltaTime;
					Vector old = particle.Position;
					particle.Position = (particle.Position * (2 - Damping)) - (particle.PositionOld * (1-Damping)) + Acceleration;
					particle.PositionOld = old;
					particle.Force = Vector();
				}
			}
		}
	}
}
void WaterEngineParticle::UpdateGrid()
{
	for(int x = 0;x < ParticleTree::Width;++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = ParticleList.GetNode(x, y, z);
				for (int i = 0; i < node.ParticleCount; ++i)
				{
					auto particle = node.GetParticle(i);
					float xgrid = std::floor(particle.Position.X / ParticleTree::Size);
					float ygrid = std::floor(particle.Position.Y / ParticleTree::Size);
					float zgrid = std::floor(particle.Position.Z / ParticleTree::Size);
					if (xgrid != x || ygrid != y || zgrid != z)
					{
						node.RemoveParticle(i);
						ParticleList.GetNode(xgrid, ygrid, zgrid).AddParticle(particle);
					}
				}
			}
		}
	}
}
void WaterEngineParticle::Update()
{
	UpdateDensity();
	UpdateForces();
	Intergrate();
	UpdateConditions();
	UpdateGrid();
}
float WaterEngineParticle::SpikyGrad(float r)
{
	return  -45.f / (3.14 * std::pow(SmoothingParam, 6.0)) * std::pow(SmoothingParam - r, 2.0);
}