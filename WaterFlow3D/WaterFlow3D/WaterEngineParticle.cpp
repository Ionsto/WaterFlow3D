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
#pragma omp parallel for //collapse(3)
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
						double Distance = (particle.Position - cparticle.Position).Magnitude();
						//Distance = std::max(0.5,Distance);
						if (Distance < SmoothingParam)
						{
							particle.Density += cparticle.Mass * Poly6(Distance);
						}
					}
				}
			}
		}
		//particle.Pressure = std::max(0.0f,((particle.Density - particle.Density0) * particle.GasConstant));
		particle.Pressure =(((particle.Density - particle.Density0) * particle.GasConstant));
	}
}
double WaterEngineParticle::Poly6(double r)
{
	return 315.f * std::pow((SmoothingParam * SmoothingParam) - (r * r), 3.0) / (64.f * 3.14 * pow(SmoothingParam, 9.0f));
}
void WaterEngineParticle::UpdateForces()
{
#pragma omp parallel for //collapse(3)
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
						double distance = difference.Magnitude();
						if (distance != 0 && distance < SmoothingParam && cparticle.Density != 0 && particle.Density != 0)
						{
							double PressureForce = ((particle.Pressure + cparticle.Pressure)) * SpikyGrad(distance);
							difference = difference / distance;
							particle.Force -= difference * (PressureForce * cparticle.Mass / (2.0*cparticle.Density));
							cparticle.Force += difference * (PressureForce * particle.Mass / (2.0*particle.Density));
							Vector cvel = (cparticle.Position - cparticle.PositionOld) / DeltaTime;
							Vector pvel = (particle.Position - particle.PositionOld) / DeltaTime;
							double LaplaceFactor = LaplaceVisc(distance);
							Vector dv = (cvel - pvel);
							//double InteractionViscosity = (particle.Viscosity + cparticle.Viscosity) / 2.0;
							double InteractionViscosity = particle.Viscosity * cparticle.Viscosity;
							double ViscFactor =  (LaplaceFactor * InteractionViscosity * cparticle.Mass / cparticle.Density);
							double ViscFactorc = (LaplaceFactor * InteractionViscosity * particle.Mass / particle.Density);
							Vector TanVec = dv - (difference * (difference.Dot(dv)));
							double error = TanVec.Dot(difference);
							//std::cout << "ERR:" << error << "\n";
							particle.Force += TanVec * ViscFactor;
							cparticle.Force -= TanVec * ViscFactorc;
						}
					}
				}
			}
		}
		particle.Force.Y -= particle.Mass * Gravity;
		//particle.Force += Vector(BrownianMotion(distibution),BrownianMotion(distibution),0);
	}
}
void WaterEngineParticle::UpdateConditions()
{
	for (int x = 0; x < ParticleTree::Width; ++x)
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
void WaterEngineParticle::UpdateConditionsAxis(double &x, double &y, double &z, double &ox, double &oy, double &oz,double lim)
{
	double dx = x - ox;
	double dy = y - oy;
	double dz = z - oz;
	ox = lim;
	x = lim-dx*Restitution;
	y -= dy * BoundaryFriction;
	z -= dz * BoundaryFriction;
}
void WaterEngineParticle::UpdateConditionsNode(ParticleNode & node)
{
	for (int i = 0; i < node.ParticleCount; ++i)
	{
		auto & Particle = node.GetParticle(i);
		if (Particle.Position.X < 0)
		{
			UpdateConditionsAxis(
				Particle.Position.X,
				Particle.Position.Y,
				Particle.Position.Z,
				Particle.PositionOld.X,
				Particle.PositionOld.Y,
				Particle.PositionOld.Z,
				0
			);
		}
		if (Particle.Position.Y < 0)
		{
			UpdateConditionsAxis(
				Particle.Position.Y,
				Particle.Position.X,
				Particle.Position.Z,
				Particle.PositionOld.Y,
				Particle.PositionOld.X,
				Particle.PositionOld.Z,
				0
			);
		}
		if (Particle.Position.X > ParticleList.TotalWidth)
		{
			UpdateConditionsAxis(
				Particle.Position.X,
				Particle.Position.Y,
				Particle.Position.Z,
				Particle.PositionOld.X,
				Particle.PositionOld.Y,
				Particle.PositionOld.Z,
				ParticleList.TotalWidth	
			);
		}
		if (Particle.Position.Y > ParticleList.TotalWidth)
		{
			UpdateConditionsAxis(
				Particle.Position.Y,
				Particle.Position.X,
				Particle.Position.Z,
				Particle.PositionOld.Y,
				Particle.PositionOld.X,
				Particle.PositionOld.Z,
			ParticleList.TotalWidth	
			);
		}
	}
}
void WaterEngineParticle::UpdateFriction()
{
	for (int x = 0; x < ParticleTree::Width; ++x)
	{
		for (int y = 0; y < ParticleTree::Width; ++y)
		{
			for (int z = 0; z < ParticleTree::Height; ++z)
			{
				auto & node = ParticleList.GetNode(x, y, z);
				UpdateFrictionNode(node);
			}
		}
	}
}
void WaterEngineParticle::UpdateFrictionNode(ParticleNode & node)
{
	for (int i = 0; i < node.ParticleCount; ++i)
	{
		auto & Particle = node.GetParticle(i);
		if (Particle.Position.X < 0)
		{
			Vector vel = Particle.Position - Particle.PositionOld;
			double FrictionMax = BoundaryFriction * std::abs(Particle.Force.X / DeltaTime);
			Particle.Force.Y += std::copysignf(-vel.Y, std::min(std::abs(Particle.Force.Y), FrictionMax));
			Particle.Force.Z += std::copysignf(-vel.Z, std::min(std::abs(Particle.Force.Z), FrictionMax));
		}
		if (Particle.Position.Y < 0)
		{
			Vector vel = Particle.Position - Particle.PositionOld;
			double FrictionMax = BoundaryFriction * std::abs(Particle.Force.Y);
			Particle.Force.X += std::copysignf(-vel.X, std::min(std::abs(Particle.Force.X), FrictionMax));
			Particle.Force.X += std::copysignf(-vel.X, std::min(std::abs(Particle.Force.X), FrictionMax));
//			Particle.Force.Z += std::copysignf(-vel.Z, std::min(std::abs(vel.Z / (DeltaTime * DeltaTime)), FrictionMax));
//			Particle.PositionOld.X += vel.X * BoundaryFriction;
		}
		if (Particle.Position.X > ParticleList.TotalWidth)
		{
			Vector vel = Particle.Position - Particle.PositionOld;
			double FrictionMax = BoundaryFriction * std::abs(vel.X / DeltaTime);
			Particle.Force.Y += std::copysignf(-vel.Y, std::min(std::abs(vel.Y / DeltaTime), FrictionMax));
			Particle.Force.Z += std::copysignf(-vel.Z, std::min(std::abs(vel.Z / DeltaTime), FrictionMax));
		}
		if (Particle.Position.Y > ParticleList.TotalWidth)
		{
			Vector vel = Particle.Position - Particle.PositionOld;
			double FrictionMax = BoundaryFriction * std::abs(vel.Y / DeltaTime);
			Particle.Force.X += std::copysignf(-vel.X, std::min(std::abs(vel.X / DeltaTime), FrictionMax));
			Particle.Force.Z += std::copysignf(-vel.Z, std::min(std::abs(vel.Z / DeltaTime), FrictionMax));
		}
	}
}void WaterEngineParticle::Intergrate()
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
					particle.Position = particle.Position + ((particle.Position - particle.PositionOld) * (1-Damping)) + Acceleration;
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
					int xgrid = (std::floor(particle.Position.X / ParticleTree::Size));
					int ygrid = (std::floor(particle.Position.Y / ParticleTree::Size));
					int zgrid = (std::floor(particle.Position.Z / ParticleTree::Size));
					if (!ParticleList.InBounds(xgrid, ygrid, zgrid))
					{
						particle.Position.Clamp(Vector(), Vector(ParticleTree::TotalWidth, ParticleTree::TotalWidth, ParticleTree::TotalHeight));
						//node.RemoveParticle(i);
						std::cout << "Particle out of world - removed\n";
						//i--;
						//continue;
						xgrid = std::clamp(static_cast<int>(std::floor(particle.Position.X / ParticleTree::Size)),0,ParticleTree::Width-1);
						ygrid = std::clamp(static_cast<int>(std::floor(particle.Position.Y / ParticleTree::Size)),0,ParticleTree::Width-1);
						zgrid = std::clamp(static_cast<int>(std::floor(particle.Position.Z / ParticleTree::Size)),0,ParticleTree::Height-1);
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
	UpdateFriction();
	Intergrate();
	UpdateConditions();
	UpdateGrid();
}
double WaterEngineParticle::SpikyGrad(double r)
{
	return  -45.f / (3.14 * std::pow(SmoothingParam, 6.0)) * std::pow(SmoothingParam - r, 2.0);
}
double WaterEngineParticle::LaplaceVisc(double r)
{
	return 45.f / (3.14 * pow(SmoothingParamViscosity, 6.0)) * (SmoothingParamViscosity - r);
}

void WaterEngineParticle::AddParticle(Vector pos, double mass)
{
	Particle p;
	p.Mass = mass;
	p.Position = pos;
	p.PositionOld = pos;
	ParticleList.AddParticle(p);
}
void WaterEngineParticle::AddParticle(Particle p)
{
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
