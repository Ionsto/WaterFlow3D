#include "WaterEngineParticle.h"


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
					int x = sx + x;
					int y = sy + y;
					int z = sz + z;
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