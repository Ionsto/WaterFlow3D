#include "ParticleTree.h"



ParticleTree::ParticleTree()
{
}


ParticleTree::~ParticleTree()
{
}
std::array<std::tuple <decltype(ParticleTree::RawData)::iterator, decltype(ParticleTree::RawData)::iterator>, 6> ParticleTree::GetNeigbours(int x, int y)
{
	//Oof
	std::array<std::tuple <decltype(ParticleTree::RawData)::iterator, decltype(ParticleTree::RawData)::iterator>, 6> result;
	for (int x = -1; x < 1; ++x)
	{
		for (int y = -1; y < 1; ++y)
		{
			for (int z = -1; z < 1; ++z)
			{
				if (x == 0 && y == 0 && z == 0)
				{continue;}
				if (!InBounds(x, y, z))
				{continue;}

			}
		}
	}
	return result;
}