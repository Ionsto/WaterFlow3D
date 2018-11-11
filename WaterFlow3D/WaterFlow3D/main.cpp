#include "GameManager.h"
#include "VoxelTree.h"
#include "WaterEngine.h"
#include "WaterEngineBinary.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdio.h>  
#include <stdlib.h>  

void PrintWaterSim(VoxelTree & tree,std::ofstream & out) {
	for (int z = 0; z < tree.Height; ++z) {
		for (int y = 0; y < tree.Width; ++y) {
			for (int x = 0; x < tree.Width; ++x) {
				VoxelData & voxel = tree.GetValue(x, y, z);
				out << (voxel.Type == VoxelData::VoxelType::Water ? 1 : 0) <<",";
			}
			out <<";"<< std::endl;
		}
	}
	out << "---" << std::endl;
}
void OpenPlotter() {
	char   psBuffer[128];
	FILE   *pPipe;

	/* Run DIR so that it writes its output to a pipe. Open this
	 * pipe with read text attribute so that we can read it
	 * like a text file.
	 */

	if ((pPipe = _popen("python plot.py", "rt")) == NULL)
		exit(1);

	/* Read pipe until end of file, or an error occurs. */

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
void TestWaterSim()
{
	std::cout << "Starting water sim" << std::endl;
	auto OutFile = std::ofstream("out.txt");
	VoxelTree tree;
	WaterEngineBinary waterengine;
	for (int t = 0; t < 200; ++t)
	{
		waterengine.Update(tree);
		PrintWaterSim(tree, OutFile);
	}
	std::cout << "Finished" << std::endl;
	OpenPlotter();
	char input;
	std::cin >> input;
}
int main(int argc, char **args) {
	TestWaterSim();
}