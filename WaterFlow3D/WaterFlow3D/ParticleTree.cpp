#include "ParticleTree.h"



ParticleTree::ParticleTree()
{
}


ParticleTree::~ParticleTree()
{
}
std::array<decltype(ParticleTree::RawData),6> ParticleTree::GetNeigbours(int x, int y)
{
	//Oof
	int r = 0;
	std::array<decltype(ParticleTree::RawData),6> result;
	for (int dx = -1; dx < 1; ++dx)
	{
		for (int y = -1; y < 1; ++y)
		{
			for (int z = -1; z < 1; ++z)
			{
				int x = sx + dx;
				int y = sy + dy;
				int z = sz + dz:
				auto toappend = GetNode(x,y,z);
				if (dx == 0 && dy == 0 && dz == 0)
				{continue;}
				if (!InBounds(x, y, z))
				{continue;}
				result[r] = toappend;
			}
		}
	}
	return result;
}


