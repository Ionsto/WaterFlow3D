#include "WaterEngine.h"
#include <algorithm>


WaterEngine::WaterEngine()
{
}


WaterEngine::~WaterEngine()
{
}

#pragma optimize( "", on )
void WaterEngine::PopulateNode(Vector NodePos,Particle particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.X), static_cast<int>(NodePos.Y));
	Vector Diff = particle.Position - NodePos;
	Vector distance = Diff.Abs();
	double Weighting = (1 - distance.X) * (1 - distance.Y);
	double WeightingX = copysignf(1 - distance.Y,-Diff.X);
	double WeightingY = copysignf(1 - distance.X,-Diff.Y);
	//double WeightingXY = copysignf(copysignf(1,-Diff.X),-Diff.Y);
	node.Mass += Weighting * particle.Mass;
	node.Force_Internal.X += particle.Stress.DX.X * WeightingX;
	node.Force_Internal.X += particle.Stress.DY.X * WeightingY;
	node.Force_Internal.Y += particle.Stress.DY.Y * WeightingY;
	node.Force_Internal.Y += particle.Stress.DX.Y * WeightingX;

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
#pragma optimize( "", on)
void WaterEngine::UpdateParticlesNode(Vector NodePos,Particle & particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.X), static_cast<int>(NodePos.Y));
	Vector Diff = particle.Position - NodePos;
	Vector distance = Diff.Abs();
	double Weighting = (1 - distance.X) * (1 - distance.Y);
	particle.Acceleration += node.Acceleration * Weighting;
//	Weighting = 1;
	double WeightingX = copysignf(1 - distance.Y,-Diff.X);
	double WeightingY = copysignf(1 - distance.X,-Diff.Y);
	double dxdx = node.Velocity.X * WeightingX;
	double dydy = node.Velocity.Y * WeightingY;
	//double WeightingXY = copysignf(copysignf(1,-Diff.X),-Diff.Y);
	double dxdy = 0.5* ((node.Velocity.X * WeightingY) + (node.Velocity.Y * WeightingX));
	particle.StrainRate.DX.X += dxdx;
	particle.StrainRate.DX.Y += dxdy;
	particle.StrainRate.DY.X += dxdy;
	particle.StrainRate.DY.Y += dydy;
}
#pragma optimize( "", on )
void WaterEngine::ApplyBoundary()
{
	/*
	for (int y = 0; y < grid.SizeY; ++y)
	{
		for (int x : {0,1, grid.SizeX-1, grid.SizeX-2})
		{
			auto & voxel = grid.Get(x, y);
			voxel.Velocity = Vector();
			voxel.Acceleration = Vector();
		}
	}
	for (int x = 0; x < grid.SizeX; ++x)
	{
		for (int y : {0,1, grid.SizeY-1,grid.SizeY-2})
		{
			auto & voxel = grid.Get(x, y);
			voxel.Velocity = Vector();
			voxel.Acceleration = Vector();
		}
	}*/
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		//particle.Position.Clamp(Vector(0, 0), Vector(grid.SizeX, grid.SizeY));
		//particle.PositionOld.Clamp(Vector(0, 0), Vector(grid.SizeX, grid.SizeY));
		//if (particle.Position.X < 1 || particle.Position.Y < 2 || particle.Position.X > grid.SizeX - 2  || particle.Position.Y - 2 > grid.SizeY) {
		if (particle.Position.X < 1) {
			particle.Velocity.X = std::max(particle.Velocity.X, 0.0);
		}
		if (particle.Position.Y < 1 && particle.Velocity.Y < 0) {
			particle.PositionOld = particle.Position;
			particle.Momentum.Y = std::max(particle.Momentum.Y, 0.0);
		}
	}

}
#pragma optimize( "", on )
void WaterEngine::RemoveOutOfBounds()
{
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		if (particle.Position.X < 0 || particle.Position.Y < 0 || particle.Position.X > grid.SizeX || particle.Position.Y > grid.SizeY) {
			particle_list.RemoveParticle(i--);
		}
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
	ApplyBoundary();
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
#pragma optimize( "", on )
void WaterEngine::ApplyForces() {
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		particle.Force = Vector();
		particle.Force += Vector(0, -9.82) * particle.Mass;
		Vector atractor = Vector(50, 50);
		Vector diff = particle.Position - atractor;
		double distance = diff.Magnitude();
		if (distance != 0)
		{
		//	particle.Force -= diff * 40.0* (particle.Mass/(distance*distance));
		}
		particle.Stress.DX.X += -particle.YoungsModulus * particle.StrainRate.DX.X * DeltaTime;
		particle.Stress.DX.Y += -particle.ShearModulus * particle.StrainRate.DX.Y * DeltaTime;
		particle.Stress.DY.Y += -particle.YoungsModulus * particle.StrainRate.DY.Y * DeltaTime;
		particle.Stress.DY.X += -particle.ShearModulus * particle.StrainRate.DY.X * DeltaTime;
//		particle.Stress = particle.Strain * particle.YoungsModulus;
	}
}
#pragma optimize( "", on )
void WaterEngine::IntergrateParticles() {
	static double Damping = 0.0001;
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
void WaterEngine::Update(double dtreal)
{
	dtacc += dtreal;
	int simcount = 0;
	while (dtacc >= DeltaTime)
	//for(int i  = 0;i < 60;++i)
	{
		ResetGrid();
		PopulateGrid();
		ApplyBoundary();
		ApplyForces();
		Intergrate();
		ApplyBoundary();
		RemoveOutOfBounds();
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