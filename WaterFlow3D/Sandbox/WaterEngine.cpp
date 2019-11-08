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
double WaterEngine::WeightAxis(double x)
{
	return 1.0 - std::abs(x / grid.GridDim);
}
double WaterEngine::WeightGradAxis(double x)
{
	if (x == 0)
	{
		return 0;
	}
	return std::copysign(-1.0, x) * grid.GridDim;
}

double WaterEngine::Weight(glm::dvec2 distance)
{
	return WeightAxis(distance.x) * WeightAxis(distance.y);
}
glm::dvec2 WaterEngine::WeightGrad(glm::dvec2 distance)
{
	return glm::vec2(WeightGradAxis(distance.x) * WeightAxis(distance.y),
	WeightGradAxis(distance.y) * WeightAxis(distance.x));
}

//#pragma optimize( "", off )
void WaterEngine::PopulateNode(glm::dvec2 NodePos,Particle p) {
	auto& g = grid.Get(static_cast<int>(NodePos.x), static_cast<int>(NodePos.y));
	auto d = p.Position - (NodePos * grid.GridDim);
    double weight = Weight(d);
    auto weightgrad = WeightGrad(d);
    g.Mass += p.Mass * weight;
    g.Velocity += p.Velocity * p.Mass * weight;
    g.Force += (p.Stress * weightgrad);
    g.Force += p.Force * weight;
}
#pragma optimize( "", on )
void WaterEngine::PopulateGrid(){
#pragma omp parallel for
    for(int i = 0; i < particle_list.ParticleCount;++i)
    {
		auto& particle = particle_list.GetParticle(i);
        if(particle.Position.x < 0 || particle.Position.x > grid.RealSize || particle.Position.y < 0 || particle.Position.y > grid.RealSize)
        {
            particle_list.RemoveParticle(i);
            i -= 1;
            continue;
        }
        int GridX = std::floor(particle.Position.x / grid.GridDim);
        int GridY = std::floor(particle.Position.y / grid.GridDim);
		glm::dvec2 NodePos = glm::dvec2(GridX, GridY);
        for(int x = 0;x < 2;++x)
        {
            for(int y = 0;y < 2;++y)
            {
				PopulateNode(NodePos + glm::dvec2(x,y), particle);
            }
        }
    }
	for (auto& g : grid.Raw_Data)
	{
        if(g.Mass != 0)
        {
            g.Velocity /= g.Mass;
            g.Acceleration = g.Force / g.Mass;
        }
    }
}
#pragma optimize( "", off)
void WaterEngine::UpdateParticlesNode(glm::dvec2 NodePos,Particle & p) {
	auto& g = grid.Get(static_cast<int>(NodePos.x), static_cast<int>(NodePos.y));
	auto d = p.Position - (NodePos * grid.GridDim);
    float weight = Weight(d);
    auto weightgrad = WeightGrad(d);
    p.Velocity += g.Velocity * weight;
    p.Acceleration += g.Acceleration * weight;
    p.StrainRate[0][0] += g.Velocity.x * weightgrad.x;
    p.StrainRate[1][1] += g.Velocity.y * weightgrad.y;
    p.StrainRate[1][0] += 0.5 * (g.Velocity.x * weightgrad.y) + (g.Velocity.y * weightgrad.x);
    p.StrainRate[0][1] = p.StrainRate[1][0];
}
#pragma optimize( "", off )
void WaterEngine::ApplyBoundary()
{
    float Friction = 0.1;
    float FallFriction = 0.01;
    for(int x = 0; x < grid.GridSize;++x){
        for(int d = 0;d < 1;++d){
            //Floor 
            grid.Get(x,d).Velocity.y = 0;
            grid.Get(x,d).Velocity.x *= Friction;
            //Ceiling
            grid.Get(x,grid.GridSize - (1 + d)).Velocity.y = 0;
            grid.Get(x,grid.GridSize - (1 + d)).Velocity.x *= Friction;
            //Left wall
            grid.Get(d,x).Velocity.x = 0; 
            grid.Get(d,x).Velocity.y *= Friction;
            //Right wall
            grid.Get(grid.GridSize - (1 + d),x).Velocity.x = 0;
            grid.Get(grid.GridSize - (1 + d),x).Velocity.y *= Friction;
        }
    }
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
        int GridX = std::floor(particle.Position.x / grid.GridDim);
        int GridY = std::floor(particle.Position.y / grid.GridDim);
		glm::dvec2 NodePos = glm::dvec2(GridX, GridY);
        particle.Velocity = glm::vec2(0);
        particle.Acceleration = glm::vec2(0);
        particle.StrainRate = glm::mat2x2(0);
        for(int x = 0;x < 2;++x)
        {
            for(int y = 0;y < 2;++y)
            {
                if(GridX+x >=0 && GridX+x < grid.GridSize)
                {
                    if(GridY+y >=0 && GridY+y < grid.GridSize)
                    {
						UpdateParticlesNode(NodePos + glm::dvec2(x, y), particle);
                    }
                }
            }
        }
	}
}
void WaterEngine::ResetGrid() {
	std::fill(grid.Raw_Data.begin(), grid.Raw_Data.end(), Voxel());

}
void WaterEngine::Intergrate() {
//	IntergrateGrid();
//	UpdateParticles();
//	IntergrateParticles();
}
void WaterEngine::IntergrateGrid() {
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
		particle.Force += glm::vec2(0, -9.8 * particle.Mass);
		//Plain stress
	}
}
#pragma optimize( "", on )
void WaterEngine::IntergrateParticles() {
	for (int i = 0; i < particle_list.ParticleCount; ++i)
	{
		auto& particle = particle_list.GetParticle(i);
		particle.Position += particle.Velocity * DeltaTime;
		particle.Velocity += particle.Acceleration * DeltaTime;
		particle.Strain += particle.StrainRate * DeltaTime;
//		particle.Force = glm::vec2(0, -9.8 * particle.Mass);
		float prefix = (particle.YoungsModulus) / (1 - (particle.PoissonsRatio * particle.PoissonsRatio));
		particle.Stress[0][0] = prefix * (particle.Strain[0][0] + (particle.PoissonsRatio * particle.Strain[1][1]));
		particle.Stress[1][1] = prefix * (particle.Strain[1][1] + (particle.PoissonsRatio * particle.Strain[0][0]));
		float meanshearstrain = (particle.Strain[0][1] + particle.Strain[1][0]) / 2.0;
		float deltaxy = ((0.5 * particle.YoungsModulus) / (1.0 + particle.PoissonsRatio)) * meanshearstrain;
		particle.Stress[0][1] = deltaxy;
		particle.Stress[1][0] = deltaxy;
	}
}
void WaterEngine::Update(double dtreal)
{
	dtacc += dtreal;
	constexpr static float UpdateRate = 1e-3;
	int simcount = floor(dtacc / DeltaTime);
	//while (dtacc >= DeltaTime)
	//for(int i  = 0;i < 1;++i)
	for(int i = 0;i < std::min(simcount,100);++i)
	{
		unsigned int rv = _clearfp();
		ResetGrid();
		PopulateGrid();
		ApplyBoundary();
		UpdateParticles();
		ApplyForces();
		IntergrateParticles();
		RemoveOutOfBounds();
		//ApplyBoundary();
		dtacc -= UpdateRate;// DeltaTime;
/*		unsigned int fpeRaised = fetestexcept( FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW );
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
		*/
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