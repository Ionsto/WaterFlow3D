#include "GameManager.h"
#include "VoxelTree.h"
//#include "WaterEngine.h"
//#include "WaterEngineBinary.h"
#include "WaterEngineEnergy.h"
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h> 
#include <stdlib.h>  

void PrintWaterSim(VoxelTree & tree,WaterEngineEnergy & waterengine,std::ofstream & out) {
	for (int z = 0; z < tree.Height; ++z) {
		for (int y = 0; y < tree.Width; ++y) {
			for (int x = 0; x < tree.Width; ++x) {
				VoxelData & voxel = tree.GetValue(x, y, z);
				out << (voxel.Type == VoxelData::VoxelType::Water ? 1 : 0) << ",";
				//out << waterengine.CaclulateVoxelEnergy(tree,x,y,z,tree.SwapBufferIndex) << ",";
				//out << voxel.Pressure << ",";
			}
			out <<";"<< std::endl;
		}
	}
	out << "---" << std::endl;
}
void OpenPlotter() {
	if constexpr (_WIN32) {
		char   psBuffer[128];
		FILE   *pPipe;

		if ((pPipe = _popen("python plot.py", "rt")) == NULL)
			exit(1);

		while (fgets(psBuffer, 128, pPipe))
		{
			printf(psBuffer);
		}

		/* Close pipe and print return value of pPipe. */
		if (feof(pPipe))
		{
			printf("\nProcess returned %d\n", _pclose(pPipe));
		}
		else
		{
			printf("Error: Failed to read the pipe to the end.\n");
		}
	}
}
void TestWaterSim()
{
	std::cout << "Starting water sim" << std::endl;
	{
		auto OutFile = std::ofstream("out.txt", std::ofstream::out | std::ofstream::trunc);
		VoxelTree tree;
		WaterEngineEnergy waterengine;
		static constexpr int tmax = 50;
		for (int t = 0; t < tmax; ++t)
		{
			tree.GetValue((int)tree.Width / 2, (int)tree.Width / 2, 0).Type = VoxelData::VoxelType::Water;
			waterengine.Update(tree);
			PrintWaterSim(tree, waterengine, OutFile);
			float energy = waterengine.CaclulateEnergy(tree, tree.SwapBufferIndex);
			float entropy = waterengine.CaclulateEntropy(tree, tree.SwapBufferIndex, energy);
			std::cout << (float)t*100.0 / tmax << "|" << energy << ":" << entropy << std::endl;
		}
		std::cout << "Finished" << std::endl;
	}
	OpenPlotter();
	//char input;
	//std::cin >> input;
}
int main(int argc, char **args) {
	TestWaterSim();
}
