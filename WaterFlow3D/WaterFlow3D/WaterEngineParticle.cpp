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
		for(int y = 0;y < ParticleTree::Width;++y)
		{
			for(int z = 0;z < ParticleTree::Height;++z)
			{
				auto & node = ParticleList.GetNode(x, y, z);
				for(int i = 0;i 
		}
	}
}
