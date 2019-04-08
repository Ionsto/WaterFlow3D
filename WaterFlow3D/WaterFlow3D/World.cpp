#include "World.h"



World::World()
{
}


World::~World()
{
}

void World::Update(float dtreal)
{
	dtacc += dtreal;
	int simcount = 0;
	while (dtacc >= DeltaTime)
	{
		//for (int x = 0; x < 10; ++x)
		//{
			static constexpr int tmax = 1000;
			static constexpr double WaterMass = 2000;
			static constexpr double WaterFlowRate = 2000;
			//if ( t % static_cast<int>(100 / (WaterMass * waterengine.DeltaTime)) == 0) {
			//Vector pos = Vector(5 + (rand() % 10) / 15.0f, 55);
				//AddWater(pos);
			//}
			waterengine.Update();
			t++;
		//}
		WaterUpdateTicker = 0;
		dtacc -= DeltaTime;
		simcount++;
	}
	//std::cout << "simcount:" << simcount << "\n";
}
void World::AddSand(Vector pos) {
	Particle p;
	p.Mass = 900;
	p.Sand = true;
	p.Viscosity = 600;
	p.Density0 = 1000;
	p.GasConstant = 800;
	p.Position = pos;
	p.PositionOld = pos;
	p.Type = 1;
	waterengine.AddParticle(p);
}
void World::AddWater(Vector pos) {
	Particle p;
	p.Mass = 100;
	p.Sand = false;
	p.Viscosity = 5;
	p.Density0 = 200;
	p.GasConstant = 500;
	p.Position = pos;
	p.PositionOld = pos;
	p.Type = 0;
	waterengine.AddParticle(p);
}