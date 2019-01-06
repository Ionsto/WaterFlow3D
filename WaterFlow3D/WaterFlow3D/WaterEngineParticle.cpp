#include "WaterEngineParticle.h"
#include "Vector.h"
#include <fstream>
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
					if (!ParticleList.InBounds(x,y,z))
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
		particle.Pressure = ((particle.Density - Density0) * GasConstant);
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
					if (!ParticleList.InBounds(x, y, z))
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
		particle.Force.Y -= particle.Mass * Gravity;
	}
}
void WaterEngineParticle::UpdateConditions()
{
	for(int x = 0;x < ParticleTree::Width;++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = ParticleList.GetNode(x, y, z);
				UpdateConditionsNode(node);
			}
		}
	}
}
void WaterEngineParticle::UpdateConditionsNode(ParticleNode & node)
{
	for(int i = 0;i < node.ParticleCount;++i)
	{
		auto & Particle = node.GetParticle(i); 
		if(Particle.Position.X < 0)
		{
			float dx = Particle.Position.X - Particle.PositionOld.X;
			Particle.PositionOld.X = 0;
			Particle.Position.X = -dx*Restitution;
		}
		if(Particle.Position.Y < 0)
		{
			float dy = Particle.Position.Y - Particle.PositionOld.Y;
			Particle.PositionOld.Y = 0;
			Particle.Position.Y = -dy*Restitution;
		}
		if(Particle.Position.X > ParticleList.TotalWidth)
		{
			float dx = Particle.Position.X - Particle.PositionOld.X;
			Particle.PositionOld.X = ParticleList.TotalWidth;
			Particle.Position.X = ParticleList.TotalWidth-dx*Restitution;
		}
		if(Particle.Position.Y > ParticleList.TotalWidth)
		{
			float dy = Particle.Position.Y - Particle.PositionOld.Y;
			Particle.PositionOld.Y = ParticleList.TotalWidth;
			Particle.Position.Y = ParticleList.TotalWidth-dy*Restitution;
		}
	}
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
				for (int i = 0; i < node.ParticleCount && i >= 0; ++i)
				{
					auto particle = node.GetParticle(i);
					float xgrid = std::floor(particle.Position.X / ParticleTree::Size);
					float ygrid = std::floor(particle.Position.Y / ParticleTree::Size);
					float zgrid = std::floor(particle.Position.Z / ParticleTree::Size);
					if (!ParticleList.InBounds(xgrid, ygrid, zgrid))
					{
						node.RemoveParticle(i);
						continue;
					}
					if (xgrid != x || ygrid != y || zgrid != z)
					{
						node.RemoveParticle(i);
						ParticleList.GetNode(xgrid, ygrid, zgrid).AddParticle(particle);
						i--;
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
void WaterEngineParticle::AddParticle(Vector pos, float mass)
{
	Particle p;
	p.Mass = mass;
	p.Position = pos;
	p.PositionOld = pos;
	ParticleList.AddParticle(p);
}
void WaterEngineParticle::Print(int t, std::ofstream & out)
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
					auto & ParticleI = node.GetParticle(i);
					out << t << "," << ParticleI.Position.X << "," << ParticleI.Position.Y << "," << ParticleI.Position.Z << "," << ParticleI.Density << "\n";
				}
			}
		}
	}
}
