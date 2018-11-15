#include "VoxelTree.h"
#include <random>
class WaterEngineEnergy { 
private:
	std::random_device rd;
	std::mt19937 engine = std::mt19937(rd());
	std::uniform_int_distribution<int> offset_distribution = std::uniform_int_distribution<int>(-1, 1);
	std::bernoulli_distribution binary_distribution;
    unsigned long x=123456789, y=362436069, z=521288629;
    unsigned long rand;
    unsigned long xorshf96(void) {          //period 2^96-1
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;
        rand = x;
        x = y;
        y = z;
        z = rand ^ x ^ y;
        return z;
    }
	bool generate_offset()
	{
		return offset_distribution(engine);
	}
	bool generate_binary()
	{
		return binary_distribution(engine);
	}
	static constexpr float Gravity = 9.81;
	static constexpr float DeltaTime = 0.5;
	static constexpr float ForceThreshold = 1;
    static constexpr float DensityLookup[] = {0.1,4,10,-1};
    static constexpr float SurfaceEnergyLookup[] = {0.1,1,1,-1};
public:
	WaterEngineEnergy();
	~WaterEngineEnergy();
	float GetSurfaceEnergy(VoxelTree & tree, int x, int y, int z, int index);
	float GetSurfaceSum(VoxelTree & tree, int x, int y, int z, int index);
	float CaclulateEnergy(VoxelTree & tree, int index);
	void CopyTree(VoxelTree & tree, int source,int dest);
    void Randomise(VoxelTree & tree,int index);
    void Update(VoxelTree & tree);
};
