#include "Solver.h"
#include <iostream>
#include <chrono>
void Update(Solver & solver)
{
	solver.Update();
}
int main(int argc, char** args)
{
	constexpr const int RunTime = 600;
	Solver solver;
	solver.AddParticle(Vector(20,40));
	//auto start = std::chrono::high_resolution_clock::now();
	for(int t = 0;t < RunTime;++t)
	{
		if(t % 1 == 0){solver.AddParticle(Vector(20+(rand()%40)/15.0f,40));}
		Update(solver);
		solver.Print(t);
		//auto end = std::chrono::high_resolution_clock::now();
		//std::cout<<(end - start).count()<<"\n";
		//start = end;
	}
	return 0;
}
