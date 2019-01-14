#include "Solver.h"
#include <iostream>
#include <chrono>
#include <array>
void Update(Solver & solver)
{
	solver.Update();
}
int main(int argc, char** args)
{
	std::ofstream out("data.txt");
	std::ofstream timingout("timing.txt");
	static constexpr const int RunTime = 1000;
	constexpr const float WaterMass = 800;
	std::ios_base::sync_with_stdio(false);
	std::array<int,RunTime> TimingArray;
	Solver solver;
	solver.AddParticle(Vector(20,40), WaterMass);
	auto start = std::chrono::high_resolution_clock::now();
	for(int t = 0;t < RunTime;++t)
	{
		if(t % 10 == 0){solver.AddParticle(Vector(20+(rand()%40)/15.0f,40),WaterMass);}
		Update(solver);
		solver.Print(t,out);
		if (t % (RunTime >> 4) == 0) { std::cout << ((static_cast<float>(t) / RunTime) * 100.0) << "\n"; }
		auto end = std::chrono::high_resolution_clock::now();
		TimingArray[t] = (end - start).count();
		//std::cout<<(end - start).count()<<"\n";
		start = end;
	}
	for (int t : TimingArray)
	{
		timingout << t << "\n";
	}
	return 0;
}
