#include "WaterEngine.h"



WaterEngine::WaterEngine()
{
}


WaterEngine::~WaterEngine()
{
}
void WaterEngine::PopulateNode(Vector NodePos, Particle particle) {
	auto& node = grid.Get(NodePos.X, NodePos.Y);
	Vector Diff = NodePos - particle.Position;
	float Distance = Diff.Magnitude();
}
void WaterEngine::PopulateGrid(){
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		int xpos = floor(particle.Position.X);
		int ypos = floor(particle.Position.Y);
		Vector NodePos = Vector(xpos + 0.5, ypos + 0.5);
		for (int dx = 0; dx < 2; ++dx) {
			for (int dy = 0; dy < 2; ++dy) {
				PopulateNode(NodePos + Vector(dx,dy), particle);
			}
		}
	}
}
void WaterEngine::Intergrate() {
	static float Damping = 0;
	for (int i = 0; i < particle_list.ParticleCount;++i) {
		auto& particle = particle_list.GetParticle(i);
		Vector old = particle.Position;
		particle.Position += ((particle.Position - particle.PositionOld) * (1-Damping)) + (particle.Acceleration* DeltaTime * DeltaTime);
		particle.PositionOld = old;
	}
}
void WaterEngine::Update(float dtreal)
{
	dtacc += dtreal;
	int simcount = 0;
	while (dtacc >= DeltaTime)
	{
		PopulateGrid();
		Intergrate();
		dtacc -= DeltaTime;
		simcount++;
	}
}

void WaterEngine::AddWater(Vector pos)
{
	static constexpr float GridWidth = 1;
	int x_round = floor(pos.X / GridWidth);
	int y_round = floor(pos.Y / GridWidth);
	x_round = std::max(0, x_round);
	x_round = std::min(grid.SizeX, x_round);
	y_round = std::max(0, y_round);
	y_round = std::min(grid.SizeY, y_round);
	//grid.Get(x_round, y_round).Density = 1;
}