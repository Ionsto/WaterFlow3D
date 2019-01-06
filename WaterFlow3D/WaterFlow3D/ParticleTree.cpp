#include "ParticleTree.h"



ParticleTree::ParticleTree()
{
}


ParticleTree::~ParticleTree()
{
}

void ParticleTree::AddParticle(Particle particle)
{
	float xgrid = std::floor(particle.Position.X / ParticleTree::Size);
	float ygrid = std::floor(particle.Position.Y / ParticleTree::Size);
	float zgrid = std::floor(particle.Position.Z / ParticleTree::Size);
	if (!InBounds(xgrid, ygrid, zgrid)) {
		return;
	}
	GetNode(xgrid, ygrid, zgrid).AddParticle(particle);
}