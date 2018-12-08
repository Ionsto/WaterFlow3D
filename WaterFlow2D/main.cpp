#include "Solver.h"
#include <iostream>
void Update(Solver & solver)
{
	solver.Update();
}
int main(int argc, char** args)
{
	constexpr const int RunTime = 1000;
	Solver solver;
	std::cout<<"Starting"<<std::endl;
	for(int t = 0;t < RunTime;++t)
	{
		Update(solver);
	}
	std::cout<<"Ended"<<std::endl;
	return 0;
}
