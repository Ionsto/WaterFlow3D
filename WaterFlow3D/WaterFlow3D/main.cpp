#include "GameManager.h"
#include "WaterEngineParticle.h"
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h> 
#include <stdlib.h>  

void TestWaterSim()
{
	std::cout << "Starting water sim" << std::endl;
	{
		auto OutFile = std::ofstream("out.txt", std::ofstream::out | std::ofstream::trunc);
		WaterEngineParticle waterengine;
		static constexpr int tmax = 50;
		for (int t = 0; t < tmax; ++t)
		{
			waterengine.Update();
			//.PrintWaterSim(tree, waterengine, OutFile);
			std::cout << (float)t*100.0 / tmax << std::endl;
		}
		std::cout << "Finished" << std::endl;
	}
	//char input;
	//std::cin >> input;
}
int main(int argc, char **args) {
	TestWaterSim();
}
