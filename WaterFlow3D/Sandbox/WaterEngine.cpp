#include "WaterEngine.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <fenv.h>
#pragma STDC FENV_ACCESS ON

WaterEngine::WaterEngine()
{
	Logfile.open("log.txt");
}


WaterEngine::~WaterEngine()
{
}
double WaterEngine::WeightPolyAxis(double x)
{
	if (abs(x) <= 0.5)
	{
		return 0.75 - (x * x);
	}
	else if (-1.5 < x  && x < -0.5)

	{
		return 1.125 + (1.5*x) + (0.5 * x * x);
	}
	else if (1.5 > x && x > 0.5)
	{
		return 1.125 - (1.5*x) + (0.5 * x * x);
	}
	return 0;
//	std::cout << "Unexpected weight\n";
//	return 0;
}
double WaterEngine::WeightPolyGradAxis(double x)
{
	if (abs(x) <= 0.5)
	{
		return - 2.0 * x;
	}
	else if (-1.5 < x && x < -0.5)
	{
		return 1.5 + x;
	}
	else if (1.5 > x && x > 0.5)
	{
		return -1.5 + x;
	}
	return 0;
}

double WaterEngine::WeightPoly(glm::dvec2 pos)
{
	return WeightPolyAxis(pos.x) * WeightPolyAxis(pos.y);
	 
}
glm::dvec2 WaterEngine::WeightGradPoly(glm::dvec2 pos)
{
	return glm::dvec2(WeightPolyGradAxis(pos.x) * WeightPolyAxis(pos.y), WeightPolyGradAxis(pos.y) * WeightPolyAxis(pos.x));
}
double WaterEngine::WeightLinear(glm::dvec2 distance)
{
	return (1.0 - distance.x) * (1.0 - distance.y);
}
glm::dvec2 WaterEngine::WeightGradLinear(glm::dvec2 distance)
{
	return glm::dvec2(copysign(1.0,-distance.x), copysign(1.0,-distance.y));
}

//#pragma optimize( "", off )
void WaterEngine::PopulateNode(glm::dvec2 NodePos,Particle particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.x), static_cast<int>(NodePos.y));
	glm::dvec2 Diff = particle.Position - (NodePos);
	double Weighting = WeightPoly(Diff);
	glm::dvec2 WeightGrad = WeightGradPoly(Diff);
	node.Mass += Weighting * particle.Mass;
	node.Force_Internal += particle.Stress * WeightGrad;
	node.Force_External += particle.Force * Weighting;
	node.Velocity += particle.Momentum * Weighting;
}
#pragma optimize( "", on )
void WaterEngine::PopulateGrid(){
#pragma omp parallel for
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		double xpos = round(particle.Position.x);
		double ypos = round(particle.Position.y);
		glm::dvec2 NodePos = glm::dvec2(xpos, ypos);
		for (int dx = -GridEvalSize; dx <= GridEvalSize; ++dx) {
			for (int dy =  -GridEvalSize; dy <= GridEvalSize; ++dy) {
				if (grid.InBounds(NodePos.x + dx, NodePos.y + dy)) {
					PopulateNode(NodePos + glm::dvec2(dx,dy), particle);
				}
			}
		}
	}
/*	for (auto& voxel : grid.Raw_Data)
	{
		if (voxel.Mass != 0) {
			voxel.Velocity /= voxel.Mass;
		}
	}*/
}
double LinearWeight(glm::dvec2 distance)
{
	return (1 - distance.x) * (1 - distance.y);
}
glm::dvec2 LinearGradWeight(glm::dvec2 distance, glm::dvec2 Diff)
{
	return glm::dvec2(copysign(1.0 - distance.y, -Diff.x), copysign(1.0 - distance.x, -Diff.y));
}
#pragma optimize( "", off)
void WaterEngine::UpdateParticlesNode(glm::dvec2 NodePos,Particle & particle) {
	auto& node = grid.Get(static_cast<int>(NodePos.x), static_cast<int>(NodePos.y));
	glm::dvec2 Diff = particle.Position - (NodePos);
	double Weighting = WeightPoly(Diff);
	particle.Velocity += node.Velocity * Weighting;
	particle.Acceleration += node.Acceleration * Weighting;
	glm::dvec2 GradWeight = WeightGradPoly(Diff);
	glm::dvec2 dvdv = node.Velocity * GradWeight;
	double dxdy = 0.5* ((node.Velocity.x * GradWeight.y) + (node.Velocity.y * GradWeight.x));
	particle.StrainRate[0][0] += dvdv.x;
	particle.StrainRate[0][1] += dxdy;
	particle.StrainRate[1][0] += dxdy;
	particle.StrainRate[1][1] += dvdv.y;
}
#pragma optimize( "", off )
void WaterEngine::ApplyBoundary()
{
	for (int x = 0; x < grid.SizeX; ++x)
	{
		for (int t = 0; t < 20; ++t)
		{
			grid.Get(x, t).Velocity.y = 0;
		}
	}
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		static constexpr double damping = 0.9;
		static constexpr double friction = 0.1;
		auto& particle = particle_list.GetParticle(i);
		//particle.Position.Clamp(glm::dvec2(0, 0), glm::dvec2(grid.SizeX, grid.SizeY));
		//particle.PositionOld.Clamp(glm::dvec2(0, 0), glm::dvec2(grid.SizeX, grid.SizeY));
		//std::clamp(particle.Position.x, 3.0, static_cast<double>(grid.SizeX)-2.0);
		//std::clamp(particle.Position.y, 3.0, static_cast<double>(grid.SizeY)-3.0);
		//if (particle.Position.x < 1 || particle.Position.y < 2 || particle.Position.x > grid.SizeX - 2  || particle.Position.y - 2 > grid.SizeY) {
		if (particle.Position.y < 2) {
//			particle.Velocity *= damping;
//			particle.Position.y = 2;
			//particle.PositionOld.y += del.y * damping;
			//particle.PositionOld.x += del.x * friction;
		//	particle.PositionOld.y = 2;
		//	particle.Position.y = 2;
		}
/*		if (particle.Position.x < 2){
			particle.PositionOld.x += particle.Velocity.x * damping;
			particle.PositionOld.y += particle.Velocity.y * friction;
		}
		if (particle.Position.y > this->grid.SizeY - 3  && particle.Velocity.y > 0) {
			particle.PositionOld.y += particle.Velocity.y * damping;
			particle.PositionOld.x += particle.Velocity.x * friction;
		}
		if (particle.Position.x > this->grid.SizeX - 3  && particle.Velocity.x > 0) {
			particle.PositionOld.x += particle.Velocity.x * damping;
			particle.PositionOld.y += particle.Velocity.y * friction;
		}*/
		if (particle.Position.x < 1 || particle.Position.y < 1 || particle.Position.x > grid.SizeX-2 || particle.Position.y > grid.SizeY-2) {
			particle_list.RemoveParticle(i--);
		}
	}
	//RemoveOutOfBounds();

}
#pragma optimize( "", on )
void WaterEngine::RemoveOutOfBounds()
{
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		if (particle.Position.x < 0 || particle.Position.y < 0 || particle.Position.x > grid.SizeX || particle.Position.y > grid.SizeY) {
			particle_list.RemoveParticle(i--);
		}
	}
}

void WaterEngine::UpdateParticles(){
#pragma omp parallel for
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		particle.Acceleration = glm::dvec2(0,0);
		particle.Velocity = glm::dvec2(0,0);
		particle.StrainRate = glm::dmat2x2(0);
		double xpos = round(particle.Position.x);
		double ypos = round(particle.Position.y);
		glm::dvec2 NodePos = glm::dvec2(xpos, ypos);
		for (int dx = -GridEvalSize; dx <= GridEvalSize; ++dx) {
			for (int dy = -GridEvalSize; dy <= GridEvalSize; ++dy) {
				if (grid.InBounds(static_cast<int>(NodePos.x) + dx, static_cast<int>(NodePos.y) + dy)) {
					UpdateParticlesNode(NodePos + glm::dvec2(dx,dy), particle);
				}
			}
		}
		//std::cout <<"A:"<< particle.Acceleration.y << "\n";
		std::cout <<"V:"<< particle.Velocity.y << "\n";
		std::cout <<"S:"<< particle.Strain[1][1] << "\n";
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
//#pragma omp parallel for
	for (auto & voxel : grid.Raw_Data)
	{
		if (voxel.Mass != 0) {
			voxel.Velocity /= voxel.Mass;
			voxel.Force = voxel.Force_External + voxel.Force_Internal;
			voxel.Acceleration = voxel.Force / voxel.Mass;
			//voxel.Velocity += voxel.Acceleration * static_cast<double>(DeltaTime);
		}
		else
		{
			voxel.Acceleration = glm::dvec2();
			voxel.Velocity = glm::dvec2();
			voxel.Force = glm::dvec2();
			voxel.Acceleration = glm::dvec2();
		}
	}
}
#pragma optimize( "", off )
void WaterEngine::ApplyForces() {
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		particle.Force = glm::dvec2();
		//particle.Force += glm::dvec2{ 0, -9.82 } *particle.Mass;
		//particle.Force += glm::dvec2{ 0, -9.82 } *particle.Mass;

		glm::dvec2 diff = particle.Position - MousePull;
		double distance = glm::length(diff);
		if (distance != 0 && distance < 15.0 && MouseAttract != 0)
		{
			particle.Force -= (diff / distance)* MouseAttract* particle.Mass;
		}
		//Plain stress

		constexpr static double min_strain_rate = 1e-8;
		constexpr static double max_strain_rate = 1e-5;
		for (int x = 0; x < 2; ++x)
		{
			for (int y = 0; y < 2; ++y)
			{
				if (std::abs(particle.StrainRate[x][y]) < min_strain_rate) {
					particle.StrainRate[x][y] = 0;
				}
//				std::clamp(particle.StrainRate[x][y], -max_strain_rate, max_strain_rate);
//				std::clamp(particle.Strain[x][y], -max_strain_rate, max_strain_rate);

			}
		}
		
		/*
		particle.Strain.DY.y += particle.StrainRate.DY.y * DeltaTime;
		particle.Strain.DY.y *= 0.99;
		if (std::abs(particle.Strain.DY.y) < 1e-8) {
			particle.Strain.DY.y = 0;
		}
		particle.Stress.DY.y = -particle.youngsModulus * particle.Strain.DY.y;
		*/
		constexpr double relaxation_factor =  0.99;
		particle.Strain += (particle.StrainRate) * DeltaTime*particle.StrainFactor;
		//std::cout << particle.Strain[1][1] << "\n";
		//particle.Strain *= relaxation_factor;
		if (particle.Type == 0) {
			double TraceCauchy = particle.Stress[0][0] + particle.Stress[1][1];
			double TraceStrainRate = particle.StrainRate[0][0] + particle.StrainRate[1][1];
			if (TraceStrainRate != 0)
			{
				glm::mat2x2 StressRate =
					(particle.c1 * TraceCauchy * particle.StrainRate) +
					(particle.c2 * TraceStrainRate * particle.Stress) +
					(particle.c3 * (TraceCauchy / TraceStrainRate) * particle.Stress) +
					(particle.c4 * (particle.Stress + glm::transpose(particle.Stress)))
					;
				for (int x = 0; x < 2; ++x)
				{
					for (int y = 0; y < 2; ++y)
					{
						particle.Stress[x][y] += StressRate[x][y] * DeltaTime;
					}
				}
			}
		}
		else if (particle.Type == 1) {
			double prefix = (particle.YoungsModulus/particle.StrainFactor) / (1 - (particle.PoissonsRatio * particle.PoissonsRatio));
			particle.Stress[0][0] = prefix * (particle.Strain[0][0] + (particle.PoissonsRatio * particle.Strain[1][1]));
			particle.Stress[1][1] = prefix * (particle.Strain[1][1] + (particle.PoissonsRatio * particle.Strain[0][0]));
			double meanshearstrain = (particle.Strain[0][1] + particle.Strain[1][0]) / 2.0;
			double deltaxy = ((0.5 * particle.YoungsModulus/particle.StrainFactor) / (1.0 + particle.PoissonsRatio)) * meanshearstrain;
			//deltaxy = 0;
			particle.Stress[0][1] = deltaxy;
			particle.Stress[1][0] = deltaxy;
			if (abs(particle.Stress[0][0]) > 1e6 || abs(particle.Stress[1][1]) > 1e6)
			{
				//std::cout << "Numerical error in stress\n";
			}
		}
	}
}
#pragma optimize( "", on )
void WaterEngine::PreIntergrate() {
	for (int i = 0; i < particle_list.ParticleCount;++i) {
		auto& particle = particle_list.GetParticle(i);
//		particle.Velocity += 0.5 * particle.Acceleration * DeltaTime;
//		particle.Position += particle.Velocity * DeltaTime;
//		particle.Momentum = particle.Velocity * particle.Mass;
// Vel verlet
//		particle.Position += (particle.Velocity * DeltaTime) + (0.5 * particle.Acceleration * static_cast<double>(DeltaTime * DeltaTime));
//		particle.AccelerationOld = particle.Acceleration;
	}
}
#pragma optimize( "", on )
void WaterEngine::IntergrateParticles() {
	static double Damping = 0.000;
	for (int i = 0; i < particle_list.ParticleCount;++i) {
		Particle& particle = particle_list.GetParticle(i);
		//verlet
		glm::dvec2 old = particle.Position;

		//particle.StrainEnergy += eng[0] + eng[1];
//		particle.Position += (particle.Velocity * DeltaTime) + (particle.AccelerationOld * static_cast<double>(DeltaTime * DeltaTime));
//		particle.Velocity += (particle.Acceleration + particle.AccelerationOld) * static_cast<double>(DeltaTime * 0.5);
//		particle.Velocity *= 1 - Damping;
//		particle.AccelerationOld = particle.Acceleration;

//Vel verlet
//		particle.Velocity += (particle.Acceleration + particle.AccelerationOld) * 0.5 * DeltaTime;
//		particle.Velocity += 0.5 * particle.Acceleration * DeltaTime;
		particle.Velocity += particle.Acceleration * DeltaTime;
		particle.Position += particle.Velocity * DeltaTime;
		particle.Acceleration = glm::dvec2();
		particle.Momentum = particle.Velocity * particle.Mass;
	}
}
void WaterEngine::Update(double dtreal)
{
	dtacc += dtreal;
	constexpr static float UpdateRate = 1e-5;
	int simcount = floor(dtacc / UpdateRate);
	//while (dtacc >= DeltaTime)
	//for(int i  = 0;i < 1;++i)
	for(int i = 0;i < std::min(simcount,1);++i)
	{
		unsigned int rv = _clearfp();
		ResetGrid();
		PreIntergrate();
		PopulateGrid();
		IntergrateGrid();
		ApplyBoundary();
		UpdateParticles();
		ApplyForces();
		IntergrateParticles();
		RemoveOutOfBounds();
		//ApplyBoundary();
		dtacc -= UpdateRate;// DeltaTime;
		unsigned int fpeRaised = fetestexcept( FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW );
		if (fpeRaised != 0)
		{
			std::cout << "Floating point exception:"<<fpeRaised<< std::endl;
			if ((fpeRaised & FE_DIVBYZERO) != 0){
				std::cout << "Div by 0 error" << std::endl;
			}
			if ((fpeRaised & FE_INVALID) != 0){
				std::cout << "Invalid" << std::endl;
			}
			if ((fpeRaised & FE_INEXACT) != 0){
				std::cout << "Inexact operation" << std::endl;
			}
			if ((fpeRaised & (FE_OVERFLOW|FE_UNDERFLOW)) != 0){
				std::cout << "flow error operation" << std::endl;
			}
			throw;
		}
		//simcount++;
	}
	//std::cout << "Energy:" << CalculateEnergy() << std::endl;
	//DumpParticles();
}

void WaterEngine::AddWater(glm::dvec2 pos)
{
	Particle p;
	p.Position = pos;
	//p.PositionOld = p.Position;
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
float WaterEngine::CalculateEnergy()
{
	float KE = 0;
	float SE = 0;
	for (int i = 0; i < particle_list.ParticleCount; ++i) {
		auto particle = particle_list.GetParticle(i);
		KE += 0.5 * particle.Mass * glm::dot(particle.Velocity, particle.Velocity);
		SE += particle.StrainEnergy;
	}
	return KE;
}