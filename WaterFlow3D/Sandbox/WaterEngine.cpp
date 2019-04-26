#include "WaterEngine.h"
#include <algorithm>


WaterEngine::WaterEngine()
{
}


WaterEngine::~WaterEngine()
{
}

#pragma optimize( "", off )
void WaterEngine::PopulateNode(Vector NodePos,Particle particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.X), static_cast<int>(NodePos.Y));
	Vector Diff = particle.Position - NodePos;
	Vector distance = Diff.Abs();
	float Weighting = (1 - distance.X) * (1 - distance.Y);
	float WeightingX = copysignf(1 - distance.Y,-Diff.X);
	float WeightingY = copysignf(1 - distance.X,-Diff.Y);
	float WeightingXY = copysignf(copysignf(1,-Diff.X),-Diff.Y);
	node.Mass += Weighting * particle.Mass;
	node.Force_Internal.X += particle.Stress.DX.X * WeightingX;
	//node.Force_Internal.X += particle.Stress.DY.X * WeightingX;
	node.Force_Internal.Y += particle.Stress.DY.Y * WeightingY;
	//node.Force_Internal.Y += particle.Stress.DX.Y * WeightingY;

	node.Force_External += particle.Force * Weighting;
	node.Velocity += particle.Momentum * Weighting;
}
#pragma optimize( "", on )
void WaterEngine::PopulateGrid(){
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		int xpos = floor(particle.Position.X);
		int ypos = floor(particle.Position.Y);
		Vector NodePos = Vector(xpos, ypos);
		for (int dx = 0; dx <= 1; ++dx) {
			for (int dy = 0; dy <= 1; ++dy) {
				if (grid.InBounds(NodePos.X + dx, NodePos.Y + dy)) {
					PopulateNode(NodePos + Vector(dx,dy), particle);
				}
			}
		}
	}
	for (auto& voxel : grid.Raw_Data) {
		if (voxel.Mass != 0) {
			voxel.Velocity /= voxel.Mass;
		}
	}
}
#pragma optimize( "", off)
void WaterEngine::UpdateParticlesNode(Vector NodePos,Particle & particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.X), static_cast<int>(NodePos.Y));
	Vector Diff = particle.Position - NodePos;
	Vector distance = Diff.Abs();
	float Weighting = (1 - distance.X) * (1 - distance.Y);
	particle.Acceleration += node.Acceleration * Weighting;
//	Weighting = 1;
	float WeightingX = copysignf(1 - distance.Y,-Diff.X);
	float WeightingY = copysignf(1 - distance.X,-Diff.Y);
	float dxdx = node.Velocity.X * WeightingX;
	float dydy = node.Velocity.Y * WeightingY;
	float WeightingXY = copysignf(copysignf(1,-Diff.X),-Diff.Y);
	float dxdy = 0.5* ((node.Velocity.X * WeightingY) + (node.Velocity.Y * WeightingX));
	particle.StrainRate.DX.X += dxdx;
	particle.StrainRate.DX.Y += dxdy;
	particle.StrainRate.DY.X += dxdy;
	particle.StrainRate.DY.Y += dydy;
}
#pragma optimize( "", on )
void WaterEngine::ApplyBoundary()
{
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		if (particle.Position.X < 1 || particle.Position.Y < 1 || particle.Position.X > grid.SizeX - 2 || particle.Position.Y > grid.SizeY - 2) {
			particle_list.RemoveParticle(i--);
		}
		//particle.Position.Clamp(Vector(1,1), Vector(grid.SizeX-2, grid.SizeY-2));
		//particle.PositionOld.Clamp(Vector(1,1), Vector(grid.SizeX-2, grid.SizeY-2));
	}
}
void WaterEngine::UpdateParticles(){
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		particle.Acceleration = Vector();
		particle.StrainRate.DX = Vector();
		particle.StrainRate.DY = Vector();
		int xpos = floor(particle.Position.X);
		int ypos = floor(particle.Position.Y);
		Vector NodePos = Vector(xpos, ypos);
		for (int dx = 0; dx <= 1; ++dx) {
			for (int dy = 0; dy <= 1; ++dy) {
				if (grid.InBounds(NodePos.X + dx, NodePos.Y + dy)) {
					UpdateParticlesNode(NodePos + Vector(dx, dy), particle);
				}
			}
		}
	}
}
void WaterEngine::ResetGrid() {
	std::fill(grid.Raw_Data.begin(), grid.Raw_Data.end(), Voxel());

}
void WaterEngine::Intergrate() {
	IntergrateGrid();
	UpdateParticles();
	IntergrateParticles();
}
void WaterEngine::IntergrateGrid() {
	for (auto & voxel : grid.Raw_Data)
	{
		if (voxel.Mass != 0) {
			voxel.Force = voxel.Force_External + voxel.Force_Internal;
			voxel.Acceleration = voxel.Force / voxel.Mass;
			voxel.Velocity += voxel.Acceleration * DeltaTime;
		}
		else
		{
			voxel.Acceleration = Vector();
			voxel.Velocity = Vector();
		}
	}
}
#pragma optimize( "", off )
void WaterEngine::ApplyForces() {
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		particle.Force = Vector();
//		particle.Force += Vector(0, -9.82) * particle.Mass;
		Vector atractor = Vector(50, 50);
		Vector diff = particle.Position - atractor;
		float distance = diff.Magnitude();
		if (distance != 0)
		{
			particle.Force -= diff * 4* (particle.Mass/distance);
		}
		particle.Stress.DX.X += -particle.YoungsModulus * particle.StrainRate.DX.X * DeltaTime;
		particle.Stress.DX.Y += -particle.ShearModulus * particle.StrainRate.DX.Y * DeltaTime;
		particle.Stress.DY.Y += -particle.YoungsModulus * particle.StrainRate.DY.Y * DeltaTime;
		particle.Stress.DY.X += -particle.ShearModulus * particle.StrainRate.DY.X * DeltaTime;
//		particle.Strain.X = particle.StrainRate.DX.X * DeltaTime;
//		particle.Strain.Y = particle.StrainRate.DY.Y * DeltaTime;
//		particle.Stress = particle.Strain * particle.YoungsModulus;
	}
}
#pragma optimize( "", on )
void WaterEngine::IntergrateParticles() {
	static float Damping = 0.00;
	for (int i = 0; i < particle_list.ParticleCount;++i) {
		auto& particle = particle_list.GetParticle(i);
		Vector old = particle.Position;
		particle.Position += ((particle.Position - particle.PositionOld) * (1-Damping)) + (particle.Acceleration* DeltaTime * DeltaTime);
		particle.PositionOld = old;
		//particle.Position.Clamp(Vector(1,1), Vector(grid.SizeX-2, grid.SizeY-2));
		//particle.PositionOld.Clamp(Vector(1,1), Vector(grid.SizeX-2, grid.SizeY-2));
		particle.Velocity = (particle.Position - particle.PositionOld) / DeltaTime;
		particle.Momentum = particle.Velocity * particle.Mass;
	}
}
void WaterEngine::Update(float dtreal)
{
	dtacc += dtreal;
	int simcount = 0;
	while (dtacc >= DeltaTime)
	{
		ResetGrid();
		PopulateGrid();
		ApplyForces();
		Intergrate();
		ApplyBoundary();
		dtacc -= DeltaTime;
		simcount++;
	}
}

void WaterEngine::AddWater(Vector pos)
{
	Particle p;
	p.Position = pos;
	p.PositionOld = p.Position;
	particle_list.AddParticle(p);
}