#include "WaterEngineEnergy.h"
WaterEngineEnergy::WaterEngineEnergy()
{
    
}
WaterEngineEnergy::~WaterEngineEnergy()
{
    
}
float WaterEngineEnergy::GetSurfaceEnergy(VoxelTree & tree, int x, int y, int z, int index)
{
	x = std::min(std::max(x, 0), tree.Width);
	y = std::min(std::max(y, 0), tree.Width);
	z = std::min(std::max(z, 0), tree.Height);
	return SurfaceEnergyLookup[tree.GetValue(x, y, z, index).Type];
}
float WaterEngineEnergy::GetSurfaceSum(VoxelTree & tree, int x, int y, int z, int index)
{
	return abs(GetSurfaceEnergy(tree, x, y, z, index) - (GetSurfaceEnergy(tree, x + 1, y, z, index) +
		GetSurfaceEnergy(tree, x - 1, y, z, index) +
		GetSurfaceEnergy(tree, x, y + 1, z, index) +
		GetSurfaceEnergy(tree, x, y - 1, z, index) +
		GetSurfaceEnergy(tree, x, y, z + 1, index) +
		GetSurfaceEnergy(tree, x, y, z - 1, index)) / 6);
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
				auto & vox = tree.GetValue(x,y,z,index);
				float LocalEnergy = 0;
				float Density = DensityLookup[vox.Type];
				float SurfaceEnergy = GetSurfaceSum(tree,x+1,y,z, index);
				LocalEnergy += SurfaceEnergy;
				LocalEnergy += 0.5 * DensityLookup[vox.Type] * vox.Velocity.Dot(vox.Velocity);
				LocalEnergy += Density * Gravity * (tree.Width - y); 
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
				auto & vox = tree.GetValue(x,y,z,index);
				float Density = DensityLookup[vox.Type];
				float SurfaceEnergy = GetSurfaceSum(tree,x+1,y,z, index);
				TotalEnergy += SurfaceEnergy;
				TotalEnergy += 0.5 * DensityLookup[vox.Type] * vox.Velocity.Dot(vox.Velocity);
				TotalEnergy += Density * Gravity * (tree.Width - y); 
            }
        }
    }
	return TotalEnergy;
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
                //if(generate_binary() && generate_binary()  && abs(dx) + abs(dy) + abs(dx) == 1){
				if (!tree.InBounds(x + dx, y + dy, z + dz)) {
					continue;
				}
                //Swap with an ajecent tile
				//auto & vox = tree.GetValue(x,y,z, index);
                auto & voxswp = tree.GetValue(x+dx,y+dy,z+dz, index);
				if (vox.Type != voxswp.Type)
				{
					float DeltaSurfaceEnergy = GetSurfaceSum(tree, x, y, z, index) - GetSurfaceSum(tree, x + dx, y + dy, z + dz, index);
					vox.Velocity += Vector3F(dx, dy, dz) * (1 - std::sqrtf(2 * DeltaSurfaceEnergy / DensityLookup[vox.Type]));
					voxswp.Velocity -= Vector3F(dx, dy, dz) * (1 - std::sqrtf(2 * DeltaSurfaceEnergy / DensityLookup[voxswp.Type]));
				}
				else {

					vox.Velocity += Vector3F(dx, dy, dz);
					voxswp.Velocity -= Vector3F(dx, dy, dz);
				}
                auto swp = vox;
                vox = voxswp;
                voxswp = swp;
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
void WaterEngineEnergy::Update(VoxelTree & tree)
{
	int StartState = tree.SwapBufferIndex;
	int BestIndex = StartState ^ 1;
	int RandomIndex = 2;
	float BestEnergy = CaclulateEnergy(tree, StartState);
	float BestEntropy = CaclulateEntropy(tree,StartState,BestEnergy);
	//CopyTree(tree, StartState, RandomIndex);
	for (int i = 0; i < 10;++i) {
		CopyTree(tree, StartState, RandomIndex);
		for (int k = 0; k < 1000; ++k) {
			Randomise(tree,RandomIndex);
			float energy = CaclulateEnergy(tree,RandomIndex);
			float entropy = CaclulateEntropy(tree,StartState,BestEnergy);
			if (energy >= BestEnergy && entropy >= BestEntropy)
			{
				BestEnergy = energy;
				BestEntropy = entropy;
				CopyTree(tree, RandomIndex, BestIndex);
			}
		}
	}
	tree.SwapBuffer();
}