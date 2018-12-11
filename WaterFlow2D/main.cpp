#include "Solver.h"
#include <iostream>
void Update(Solver & solver)
{
	solver.Update();
}
int main(int argc, char** args)
{
	constexpr const int RunTime = 200;
	Solver solver;
	solver.AddParticle(Vector(20,40));
	for(int t = 0;t < RunTime;++t)
	{
		solver.AddParticle(Vector(20+(rand()%2),40));
		Update(solver);
		solver.Print(t);
	}
	return 0;
}
