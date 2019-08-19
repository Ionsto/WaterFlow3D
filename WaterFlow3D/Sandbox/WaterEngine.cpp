#include "WaterEngine.h"
#include <algorithm>

WaterEngine::WaterEngine()
{
	Logfile.open("log.txt");
}


WaterEngine::~WaterEngine()
{
}

float WaterEngine::WeightPartialLinear(Vector distance)
{
	return (1 - distance.X) * (1 - distance.Y);
//	return std::max(0.0,(1 - (distance.Dot(distance)/2)/3.0));
}
Vector WaterEngine::WeightGradPartialLinear(Vector distance)
{
//	return Vector(copysignf(-1,distance.Y), copysignf(-1,distance.X))/10;
	//return Vector(copysignf(1 - abs(distance.Y),-distance.X),copysignf(1 - abs(distance.X),-distance.Y));
//	return Vector(-distance.Y,-distance.X)/3;
//	return Vector(-RecipricalRoot2 + (distance.Y/2), -RecipricalRoot2 + (distance.X/2));
	return Vector(1, 1 - distance.Y);
//	return Vector(copysignf(1 - abs(distance.Y),-distance.Y),copysignf(1 - abs(distance.X),-distance.Y)) * 0.25;
}
float WaterEngine::WeightLinear(Vector distance)
{
	return (1 - distance.X) * (1 - distance.Y);
}
Vector WaterEngine::WeightGradLinear(Vector distance)
{
	return Vector(copysignf(0,-distance.X),copysignf(-1,-distance.Y));
}

//#pragma optimize( "", on )
void WaterEngine::PopulateNode(Vector NodePos,Particle particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.X), static_cast<int>(NodePos.Y));
	Vector Diff = particle.Position - NodePos;
	Vector distance = Diff.Abs();
	double Weighting = WeightPartialLinear(distance);//(1 - distance.X)* (1 - distance.Y);
	Vector WeightGrad = WeightGradPartialLinear(Diff);
	node.Mass += Weighting * particle.Mass;
	//node.Force_Internal.X += particle.Stress.DX.X * WeightGrad.X;
	//node.Force_Internal.X += particle.Stress.DX.Y * WeightGrad.Y;
	node.Force_Internal.Y += particle.Stress.DY.Y * WeightGrad.Y;
	//node.Force_Internal.Y += particle.Stress.DY.X * WeightGrad.X;
	node.Force_External += particle.Force * Weighting;
	node.Velocity += particle.Momentum * Weighting;
}
//#pragma optimize( "", on )
void WaterEngine::PopulateGrid(){
#pragma omp parallel for
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
float LinearWeight(Vector distance)
{
	return (1 - distance.X) * (1 - distance.Y);
}
Vector LinearGradWeight(Vector distance, Vector Diff)
{
	return Vector(copysignf(1 - distance.Y, -Diff.X), copysignf(1 - distance.X, -Diff.Y));
}
#pragma optimize( "", on)
void WaterEngine::UpdateParticlesNode(Vector NodePos,Particle & particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.X), static_cast<int>(NodePos.Y));
	Vector Diff = particle.Position - NodePos;
	Vector distance = Diff.Abs();
	double Weighting = WeightPartialLinear(distance);
	particle.Acceleration += node.Acceleration * Weighting;
//	Weighting = 1;
	Vector GradWeight = WeightGradPartialLinear(Diff);
	double dxdx = node.Velocity.X * GradWeight.X;
	double dydy = node.Velocity.Y * GradWeight.Y;
	//double WeightingXY = copysignf(copysignf(1,-Diff.X),-Diff.Y);
	double dxdy = 0.5* ((node.Velocity.X * GradWeight.Y) + (node.Velocity.Y * GradWeight.X));
	particle.StrainRate.DX.X += dxdx;
	particle.StrainRate.DX.Y += dxdy;
	particle.StrainRate.DY.X += dxdy;
	particle.StrainRate.DY.Y += dydy;
}
//#pragma optimize( "", on )
void WaterEngine::ApplyBoundary()
{
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		static constexpr float damping = 0.8;
		static constexpr float friction = 0.5;
		auto& particle = particle_list.GetParticle(i);
		//particle.Position.Clamp(Vector(0, 0), Vector(grid.SizeX, grid.SizeY));
		//particle.PositionOld.Clamp(Vector(0, 0), Vector(grid.SizeX, grid.SizeY));
		//if (particle.Position.X < 1 || particle.Position.Y < 2 || particle.Position.X > grid.SizeX - 2  || particle.Position.Y - 2 > grid.SizeY) {
		if (particle.Position.Y < 2 && particle.Velocity.Y < 0) {
			Vector del = particle.Position - particle.PositionOld;
			particle.PositionOld.Y += del.Y * damping;
			particle.PositionOld.X += del.X * friction;
			//particle.Momentum.Y = std::max(particle.Momentum.Y, 0.0);
		}
		/*if (particle.Position.X < 1 && particle.Velocity.X < 0) {
			particle.PositionOld.X = particle.Position.X;
			particle.Momentum.X = std::max(particle.Momentum.X, 0.0);
		}*/
		if (particle.Position.X < 1 || particle.Position.Y < 1 || particle.Position.X > grid.SizeX-1 || particle.Position.Y > grid.SizeY-1) {
			particle_list.RemoveParticle(i--);
		}
	}

}
//#pragma optimize( "", on )
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
#pragma omp parallel for
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
		particle.Force += Vector(0, -1.82) * particle.Mass;
		Vector atractor = Vector(50, 50);
		Vector diff = particle.Position - atractor;
		double distance = diff.Magnitude();
		if (distance != 0)
		{
			particle.Force -= diff * 40.0* (particle.Mass/(distance*distance));
		}
/*
		particle.Stress.DX.X += -particle.YoungsModulus * particle.StrainRate.DX.X * DeltaTime;
		particle.Stress.DX.Y += -particle.ShearModulus * particle.StrainRate.DX.Y * DeltaTime;
		particle.Stress.DY.Y += -particle.YoungsModulus * particle.StrainRate.DY.Y * DeltaTime;
		particle.Stress.DY.X += -particle.ShearModulus * particle.StrainRate.DY.X * DeltaTime;
*/
		//Plain stress
		particle.Strain.DY.Y += particle.StrainRate.DY.Y * DeltaTime;
		particle.Strain.DY.Y *= 0.9;
		particle.Stress.DY.Y = particle.YoungsModulus * particle.Strain.DY.Y;
		/*
		if constexpr (true) {
			float prefix = particle.YoungsModulus / (1 - (particle.PoissonsRatio * particle.PoissonsRatio));
			particle.Stress.DX.X +=  prefix * (particle.StrainRate.DX.X + particle.PoissonsRatio * particle.StrainRate.DY.Y) * DeltaTime;
			particle.Stress.DY.Y +=  prefix * (particle.StrainRate.DY.Y + particle.PoissonsRatio * particle.StrainRate.DX.X) * DeltaTime;
			float deltaxy = (0.5 * particle.YoungsModulus / (1.0 + particle.PoissonsRatio)) * particle.StrainRate.DX.Y * DeltaTime;
			particle.Stress.DX.Y += deltaxy;
			particle.Stress.DY.X += deltaxy;
		}
		else
		{
			//plain strain
			float prefix = particle.YoungsModulus / ((1 + particle.PoissonsRatio) * (1 - 2*particle.PoissonsRatio));
			float sb = 1 - particle.PoissonsRatio;
			particle.Stress.DX.X +=  prefix * (particle.StrainRate.DX.X * sb + particle.PoissonsRatio * particle.StrainRate.DY.Y) * DeltaTime;
			particle.Stress.DY.Y +=  prefix * (particle.StrainRate.DY.Y * sb + particle.PoissonsRatio * particle.StrainRate.DX.X) * DeltaTime;
			float deltaxy = prefix * (1 - 2 * particle.PoissonsRatio) / 2 * particle.StrainRate.DX.Y * DeltaTime;
			particle.Stress.DX.Y += deltaxy;
			particle.Stress.DY.X += deltaxy;
		}*/

//		particle.Stress = particle.Strain * particle.YoungsModulus;
	}
}
#pragma optimize( "", on )
//#pragma optimize( "", on )
void WaterEngine::IntergrateParticles() {
	static double Damping = 0.0;
	for (int i = 0; i < particle_list.ParticleCount;++i) {
		auto& particle = particle_list.GetParticle(i);
		particle.Velocity = (particle.Position - particle.PositionOld) / DeltaTime;
		Vector old = particle.Position;
		particle.Position += ((particle.Position - particle.PositionOld) * (1-Damping)) + (particle.Acceleration* DeltaTime * DeltaTime);
		particle.PositionOld = old;
		particle.Momentum = particle.Velocity * particle.Mass;
	}
}
void WaterEngine::Update(double dtreal)
{
	dtacc += dtreal;
	int simcount = 0;
//	while (dtacc >= DeltaTime)
	for(int i  = 0;i < 1;++i)
	{
		ResetGrid();
		PopulateGrid();
		ApplyBoundary();
		ApplyForces();
		Intergrate();
		ApplyBoundary();
		dtacc -= DeltaTime;
		simcount++;
	}
	//DumpParticles();
}

void WaterEngine::AddWater(Vector pos)
{
	Particle p;
	p.Position = pos;
	p.PositionOld = p.Position;
	particle_list.AddParticle(p);
}
void WaterEngine::DumpParticles()
{
	for (auto p : particle_list.ParticleList)
	{
		Logfile << p << ";";
	}
	Logfile << std::endl;
}