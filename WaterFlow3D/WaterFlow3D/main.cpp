#include "GameManager.h"
#include "WaterEngineParticle.h"
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h> 
#include <stdlib.h>  
#include <cmath>
#include <chrono>
void TestWaterSim()
{
	std::ios_base::sync_with_stdio(false);
	std::cout << "Starting water sim" << std::endl;
	{
		auto OutFile = std::ofstream("out.txt", std::ofstream::out | std::ofstream::trunc);
		std::ofstream timingout("timing.txt");
		WaterEngineParticle waterengine;
		static constexpr int tmax = 4000;
		auto start = std::chrono::high_resolution_clock::now();
		for (int t = 0; t < tmax; ++t)
		{
			//if (t % static_cast<int>(0.1 / waterengine.DeltaTime) == 0) { waterengine.AddParticle(Vector(5 + (rand() % 10) / 15.0f, 9,0.1 + (rand() % 10) / 15.0f), 0.03f); }
			if (t % static_cast<int>(0.1 / waterengine.DeltaTime) == 0) { waterengine.AddParticle(Vector(5 + (rand() % 10) / 15.0f, 9), 0.04f); }
			waterengine.Update();
			waterengine.Print(t, OutFile);
			//.PrintWaterSim(tree, waterengine, OutFile);
			auto end = std::chrono::high_resolution_clock::now();
			std::cout << (float)t*100.0 / tmax << std::endl;
			timingout << std::chrono::duration<double>(end-start).count() << std::endl;
			start = end;
		}
	}
	//char input;
	//std::cin >> input;
}
int main(int argc, char **args) {
	TestWaterSim();
}
