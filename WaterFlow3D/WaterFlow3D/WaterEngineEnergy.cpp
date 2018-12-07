#include "WaterEngineEnergy.h"
#include <iostream>
WaterEngineEnergy::WaterEngineEnergy()
{
	engine = std::mt19937(rd());
	offset_distribution = std::uniform_int_distribution<int>(-1, 1);
	int count[] = { 0,0,0 };
	for (int i = 0; i < 1000; ++i) {
		int val = generate_offset();
		++count[val + 1];
	}
	/*std::cout << "-1 : " << count[0] << "\n";
	std::cout << "0 : " << count[1] << "\n";
	std::cout << "1 : " << count[2] << "\n";*/
}
WaterEngineEnergy::~WaterEngineEnergy()
{
    
}
float WaterEngineEnergy::GetSurfaceEnergy(VoxelTree & tree, int x, int y, int z, int index)
{
	x = std::min(std::max(x, 0), tree.Width-1);
	y = std::min(std::max(y, 0), tree.Width-1);
	z = std::min(std::max(z, 0), tree.Height-1);
	return SurfaceEnergyLookup[tree.GetValue(x, y, z, index).Type];
}
float WaterEngineEnergy::GetSurfaceSum(VoxelTree & tree, int x, int y, int z, int index)
{
	return std::abs(GetSurfaceEnergy(tree, x, y, z, index) - (
		GetSurfaceEnergy(tree, x + 1, y, z, index) +
		GetSurfaceEnergy(tree, x - 1, y, z, index) +
		GetSurfaceEnergy(tree, x, y + 1, z, index) +
		GetSurfaceEnergy(tree, x, y - 1, z, index) +
		GetSurfaceEnergy(tree, x, y, z + 1, index) +
		GetSurfaceEnergy(tree, x, y, z - 1, index)) / 6);
}
float WaterEngineEnergy::CaclulateVoxelEnergy(VoxelTree & tree, int x, int y, int z, int index)
{
	auto & vox = tree.GetValue(x, y, z, index);
	float LocalEnergy = 0;
	float Density = DensityLookup[vox.Type];
	float SurfaceEnergy = GetSurfaceSum(tree, x,y, z, index);
	LocalEnergy += SurfaceEnergy;
	//LocalEnergy += 0.5 * DensityLookup[vox.Type] * vox.Velocity.Dot(vox.Velocity);
	LocalEnergy += Density * Gravity * y;
	auto dv = vox.Velocity - vox.PrevVelocity;
	LocalEnergy += 0.5 * DensityLookup[vox.Type] * dv.Dot(dv);
	return LocalEnergy;
}
float WaterEngineEnergy::CaclulateEntropy(VoxelTree & tree, int index,float Energy)
{
	float TotalEntropy = 0;
    float MeanEntropy = Energy/(tree.Width * tree.Width * tree.Height);
    for(int x = 0;x < tree.Width;++x)    
    {
        for(int y = 0;y < tree.Width;++y)    
        {
            for(int z = 0;z < tree.Height;++z)
            {
				float LocalEnergy = CaclulateVoxelEnergy(tree,x, y, z, index);
				TotalEntropy += std::abs(LocalEnergy - MeanEntropy);
            }
        }
    }
	return TotalEntropy/(tree.Width * tree.Width * tree.Height);
}
float WaterEngineEnergy::CaclulateEnergy(VoxelTree & tree, int index)
{
	float TotalEntropy = 0;
    float TotalEnergy = 0;
    for(int x = 0;x < tree.Width;++x)    
    {
        for(int y = 0;y < tree.Width;++y)    
        {
            for(int z = 0;z < tree.Height;++z)
            {
				TotalEnergy += CaclulateVoxelEnergy(tree, x, y, z, index);
            }
        }
    }
	return TotalEnergy;
}
void WaterEngineEnergy::DisperseVelocity(VoxelTree & tree,int index)
{
    for(int x = 0;x < tree.Width;++x)    
    {
        for(int y = 0;y < tree.Width;++y)    
        {
			for (int z = 0; z < tree.Height; ++z)
			{
				auto & vox = tree.GetValue(x, y, z, index);
			}
	    }
	}
}
void WaterEngineEnergy::Randomise(VoxelTree & tree,int index)
{
    for(int x = 0;x < tree.Width;++x)    
    {
        for(int y = 0;y < tree.Width;++y)    
        {
            for(int z = 0;z < tree.Height;++z) 
            {
                auto & vox = tree.GetValue(x,y,z, index);
				int axis = generate_offset();
				int step = generate_offset();
                int dx = axis == -1 ? step : 0;
                int dy = axis == 0 ? step : 0;
                int dz = axis == 1 ? step : 0;
				if (step != 0) {
					//if(generate_binary() && generate_binary()  && abs(dx) + abs(dy) + abs(dx) == 1){
					if (!tree.InBounds(x + dx, y + dy, z + dz)) {
						continue;
					}
					//Swap with an ajecent tile
					auto & voxswp = tree.GetValue(x + dx, y + dy, z + dz, index);
					if (vox.Type != voxswp.Type)
					{
						float DeltaSurfaceEnergy = GetSurfaceSum(tree, x, y, z, index) - GetSurfaceSum(tree, x + dx, y + dy, z + dz, index);
						vox.Velocity += Vector3F(dx, dy, dz) * (1 - std::sqrt(2 * std::abs(DeltaSurfaceEnergy) / DensityLookup[vox.Type]));
						voxswp.Velocity -= Vector3F(dx, dy, dz) * (1 - std::sqrt(2 * std::abs(DeltaSurfaceEnergy) / DensityLookup[voxswp.Type]));
					}
					else {

						vox.Velocity += Vector3F(dx, dy, dz);
						voxswp.Velocity -= Vector3F(dx, dy, dz);
					}
					/*auto swp = vox;
					vox = voxswp;
					voxswp = swp;*/
					std::swap(vox, voxswp);
				}
            }
            //}
        }
    }  
}
void WaterEngineEnergy::CopyTree(VoxelTree & tree, int source, int dest)
{
	for (int x = 0; x < tree.Width; ++x)
	{
		for (int y = 0; y < tree.Width; ++y)
		{
			for (int z = 0; z < tree.Height; ++z)
			{
				tree.GetValue(x, y, z, dest) = tree.GetValue(x, y, z, source);
			}
		}
	}
}
void WaterEngineEnergy::SwapVelocityBuffer(VoxelTree & tree)
{
	for (int x = 0; x < tree.Width; ++x)
	{
		for (int y = 0; y < tree.Width; ++y)
		{
			for (int z = 0; z < tree.Height; ++z)
			{
				tree.GetValue(x, y, z).PrevVelocity = tree.GetValue(x, y, z).Velocity;
				tree.GetValue(x, y, z).Velocity.X = 0;
				tree.GetValue(x, y, z).Velocity.Y = 0;
				tree.GetValue(x, y, z).Velocity.Z = 0;
			}
		}
	}
}
void WaterEngineEnergy::Update(VoxelTree & tree)
{
	int StartState = tree.SwapBufferIndex;
	int BestIndex = StartState ^ 1;
	int RandomIndex = 2;
	float BestEnergy = CaclulateEnergy(tree, StartState);
	float BestEntropy = CaclulateEntropy(tree,StartState,BestEnergy);
	SwapVelocityBuffer(tree);
	//CopyTree(tree, StartState, RandomIndex);
	for (int i = 0; i < 100;++i) {
		CopyTree(tree, StartState, RandomIndex);
		for (int k = 0; k < 1000; ++k) {
			Randomise(tree,RandomIndex);
			float energy = CaclulateEnergy(tree,RandomIndex);
			float entropy = CaclulateEntropy(tree,StartState,BestEnergy);//Actually inverse entropy
			if (energy <= BestEnergy && entropy <= BestEntropy)
			{
				BestEnergy = energy;
				BestEntropy = entropy;
				CopyTree(tree, RandomIndex, BestIndex);
			}
		}
	}
	tree.SwapBuffer();
	//SwapVelocityBuffer(tree);
}
