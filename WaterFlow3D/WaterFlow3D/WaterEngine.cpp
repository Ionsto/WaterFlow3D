#include "WaterEngine.h"



WaterEngine::WaterEngine()
{
}


WaterEngine::~WaterEngine()
{
}
//float WaterEngine::CalculateStress(VoxelTree & tree, int x, int y, int z) {
//	float Stress = 0;
//	for (int dx = -1; dx <= 1; ++dx) {
//		for (int dy = -1; dy <= 1; ++dy) {
//			for (int dz = -1; dz <= 1; ++dz) {
//				if (dx == 0 && dy == 0 && dz == 0) {
//					continue;
//				}
//				if (abs(dx) + abs(dy) + abs(dz) == 1) {
//					continue;
//				}
//				//Add up forces
//				if (!tree.InBounds(x + dx, y + dy, z + dz)){
//					continue;
//				}
//				auto vox = tree.GetValue(x + dx, y + dy, z + dz);
//				Stress += vox.Stress;
//			}
//		}
//	}
//	return Stress;
//}
//void WaterEngine::CalculateDensity(VoxelTree & tree, int x, int y, int z) {
//	auto & Center = tree.GetValue(x, y, z);
//	for (int dx = -1; dx <= 1; ++dx) {
//		for (int dy = -1; dy <= 1; ++dy) {
//			for (int dz = -1; dz <= 1; ++dz) {
//				if (dx== 0 && dy== 0 && dz == 0) {
//					continue;
//				}
//				if (!tree.InBounds(x + dx, y + dy, z + dz)) {
//					//Boundary condition -> noflow
//					//The resolves stress == 0 as the boundary is imovable/noflow
//					continue;
//				}
//				//Add up forces
//				auto & vox = tree.GetValue(x + dx, y + dy, z + dz);
//				if (Center.Density+vox.Density != 0.0f) {
//					float ResolvedStress = Center.Stress - vox.Stress;
//					static constexpr float Area = 1.0/6.0;
//					float DeltaDensity = 0.5 * ResolvedStress * Area * DeltaTime*DeltaTime;
//					if (-DeltaDensity > Center.Density) {
//						DeltaDensity = -Center.Density;
//					}
//					if (DeltaDensity < -vox.Density) {
//						DeltaDensity = -vox.Density;
//					}
//					Center.DensityNew -= DeltaDensity;
//					vox.DensityNew += DeltaDensity;
//				}
//			}
//		}
//	}
//}
//void WaterEngine::UpdateHydrostaticStress(VoxelTree & tree) {
//	for (int x = 0; x < tree.Width; ++x) {
//		for (int y = 0; y < tree.Width; ++y) {
//			for (int z = 0; z < tree.Height; ++z) {
//				VoxelData & voxel = tree.GetValue(x, y, z);
//				//Calculate sum of sigma_h
//				float StressNew = CalculateStress(tree, x, y, z);
//				//Add gravity force
//				StressNew += voxel.Density * Gravity;
//				voxel.StressNew = StressNew * voxel.Density / 6.0f;
//			}
//		}
//	}
//	for (int x = 0; x < tree.Width; ++x) {
//		for (int y = 0; y < tree.Width; ++y) {
//			for (int z = 0; z < tree.Height; ++z) {
//				VoxelData & voxel = tree.GetValue(x, y, z);
//				voxel.Stress = voxel.StressNew;
//			}
//		}
//	}
//}
//void WaterEngine::UpdateDensity(VoxelTree & tree) {
//	for (int x = 0; x < tree.Width; ++x) {
//		for (int y = 0; y < tree.Width; ++y) {
//			for (int z = 0; z < tree.Height; ++z) {
//				VoxelData & voxel = tree.GetValue(x, y, z);
//				voxel.DensityNew = voxel.Density;
//			}
//		}
//	}
//	for (int x = 0; x < tree.Width; ++x) {
//		for (int y = 0; y < tree.Width; ++y) {
//			for (int z = 0; z < tree.Height; ++z) {
//				VoxelData & voxel = tree.GetValue(x, y, z);
//				CalculateDensity(tree, x, y, z);
//			}
//		}
//	}
//}
//void WaterEngine::UpdateVelocity(VoxelTree & tree) {
//	for (int x = 0; x < tree.Width; ++x) {
//		for (int y = 0; y < tree.Width; ++y) {
//			for (int z = 0; z < tree.Height; ++z) {
//				VoxelData & voxel = tree.GetValue(x, y, z);
//				//Calculate sum of sigma_h
//				float StressNew = CalculateStress(tree, x, y, z);
//				//Add gravity force
//				StressNew += voxel.Density * Gravity;
//				//voxel.StressNew = StressNew;
//			}
//		}
//	}
//}
//void WaterEngine::UpdateSwapBuffer(VoxelTree & tree) {
//	for (int x = 0; x < tree.Width; ++x) {
//		for (int y = 0; y < tree.Width; ++y) {
//			for (int z = 0; z < tree.Height; ++z) {
//				VoxelData & voxel = tree.GetValue(x, y, z);
//				voxel.DensityNew = std::max(std::min(voxel.DensityNew, 1.0f), 0.0f);
//				voxel.Density = voxel.DensityNew;
//				voxel.Velocity = voxel.VelocityNew;
//				//voxel.Stress = voxel.StressNew;
//			}
//		}
//	}
//}

void WaterEngine::EnforceBoundary(VoxelTree & tree) {
	for (int xy = 0; xy < tree.Width; ++xy) {
		for (int z = 0; z < tree.Height; ++z) {
			tree.GetValueNew(xy, -1, z).Density = tree.GetValueNew(xy, 0, z).Density;
			tree.GetValueNew(-1, xy, z).Density = tree.GetValueNew(0, xy, z).Density;
			tree.GetValueNew(xy, tree.Width, z).Density = tree.GetValueNew(xy, tree.Width - 1, z).Density;
			tree.GetValueNew(tree.Width, xy, z).Density = tree.GetValueNew(tree.Width - 1, xy, z).Density;
		}
	}
	for (int x = 0; x < tree.Width; ++x) {
		for (int y = 0; y < tree.Width; ++y) {
			tree.GetValueNew(x, y, -1).Density = tree.GetValueNew(x, y, 0).Density;
			tree.GetValueNew(x, y, tree.Height).Density = tree.GetValueNew(x, y, tree.Height - 1).Density;
		}
	}
}
void WaterEngine::EnforceInflows(VoxelTree & tree) {
	static constexpr float FlowRate = 1;
	tree.GetValueNew(0, 0, 0).Density = FlowRate;
	tree.GetValueNew(0, 0, 0).Velocity = Vector3F(1,0,0);
	tree.GetValueNew(tree.Width, tree.Width, 0).Density = 0;	
}
void WaterEngine::DiffuseDensity(VoxelTree & tree) {
	static constexpr float diff = 100;
	static constexpr float a = DeltaTime * diff * tree.Width * tree.Width * tree.Height;
	for (int k = 0; k < 20; k++) {
		for (int x = 0; x < tree.Width; x++) {
			for (int y = 0; y < tree.Width; y++) {
				for (int z = 0; z < tree.Height; z++) {
					tree.GetValueNew(x, y, z).Density =
						(
							tree.GetValue(x, y, z).Density +
							a * (
								tree.GetValueNew(x + 1, y, z).Density +
								tree.GetValueNew(x, y + 1, z).Density +
								tree.GetValueNew(x, y, z + 1).Density +
								tree.GetValueNew(x - 1, y, z).Density +
								tree.GetValueNew(x, y - 1, z).Density +
								tree.GetValueNew(x, y, z - 1).Density
								)) / (1 + (6 * a));
				}
			}
		}
		EnforceInflows(tree);
		EnforceBoundary(tree);
	}
}

void WaterEngine::DiffuseVelocity(VoxelTree & tree) {
	static constexpr float diff = 1;
	static constexpr float a = DeltaTime * diff * tree.Width * tree.Width * tree.Height;
	for (int k = 0; k < 20; k++) {
		for (int x = 0; x < tree.Width; x++) {
			for (int y = 0; y < tree.Width; y++) {
				for (int z = 0; z < tree.Height; z++) {
					tree.GetValueNew(x, y, z).Velocity =
						(
							tree.GetValue(x, y, z).Velocity +
							(
								tree.GetValueNew(x + 1, y, z).Velocity +
								tree.GetValueNew(x, y + 1, z).Velocity +
								tree.GetValueNew(x, y, z + 1).Velocity +
								tree.GetValueNew(x - 1, y, z).Velocity +
								tree.GetValueNew(x, y - 1, z).Velocity +
								tree.GetValueNew(x, y, z - 1).Velocity
								)) * (a / (1 + (6 * a)));
				}
			}
		}
		EnforceInflows(tree);
		EnforceBoundary(tree);
	}
}

void WaterEngine::ProjectVelocity(VoxelTree & tree) {
	static constexpr float diff = 1;
	static constexpr float h = 1.0 / tree.Width * tree.Width * tree.Height;
	for (int x = 0; x < tree.Width; x++) {
		for (int y = 0; y < tree.Width; y++) {
			for (int z = 0; z < tree.Height; z++) {
				tree.GetValue(x, y, z).Velocity.Y =
					-0.5 * h * (
						tree.GetValueNew(x + 1, y, z).Velocity.X +
						tree.GetValueNew(x, y + 1, z).Velocity.Y +
						tree.GetValueNew(x, y, z + 1).Velocity.Z -
						tree.GetValueNew(x - 1, y, z).Velocity.X -
						tree.GetValueNew(x, y - 1, z).Velocity.Y -
						tree.GetValueNew(x, y, z - 1).Velocity.Z
					);
				tree.GetValue(x, y, z).Velocity.X = 0;
				tree.GetValue(x, y, z).Velocity.Z = 0;
			}
		}
		tree.SwapBuffer();
		EnforceInflows(tree);
		EnforceBoundary(tree);
		tree.SwapBuffer();
		for (int k = 0; k < 20; k++) {
			for (int x = 0; x < tree.Width; x++) {
				for (int y = 0; y < tree.Width; y++) {
					for (int z = 0; z < tree.Height; z++) {
						tree.GetValue(x, y, z).Velocity.X =
							(
								tree.GetValue(x, y, z).Velocity.Y +
								(
									tree.GetValue(x + 1, y, z).Velocity.X +
									tree.GetValue(x, y + 1, z).Velocity.X +
									tree.GetValue(x, y, z + 1).Velocity.X +
									tree.GetValue(x - 1, y, z).Velocity.X +
									tree.GetValue(x, y - 1, z).Velocity.X +
									tree.GetValue(x, y, z - 1).Velocity.X
									)) / (6.0);
						tree.GetValue(x, y, z).Velocity.Z =
							(
								tree.GetValue(x, y, z).Velocity.Y +
								(
									tree.GetValue(x + 1, y, z).Velocity.Z +
									tree.GetValue(x, y + 1, z).Velocity.Z +
									tree.GetValue(x, y, z + 1).Velocity.Z +
									tree.GetValue(x - 1, y, z).Velocity.Z +
									tree.GetValue(x, y - 1, z).Velocity.Z +
									tree.GetValue(x, y, z - 1).Velocity.Z
									)) / (6.0);
					}
				}
			}
			tree.SwapBuffer();
			EnforceInflows(tree);
			EnforceBoundary(tree);
			tree.SwapBuffer();
		}
		for (int x = 0; x < tree.Width; x++) {
			for (int y = 0; y < tree.Width; y++) {
				for (int z = 0; z < tree.Height; z++) {
					tree.GetValueNew(x, y, z).Velocity.X -= 0.5*(tree.GetValue(x + 1, y, z).Velocity.X - tree.GetValue(x - 1, y, z).Velocity.X) / h;
					tree.GetValueNew(x, y, z).Velocity.Y -= 0.5*(tree.GetValue(x, y + 1, z).Velocity.Y - tree.GetValue(x, y - 1, z).Velocity.Y) / h;
					tree.GetValueNew(x, y, z).Velocity.Z -= 0.5*(tree.GetValue(x, y, z + 1).Velocity.Z - tree.GetValue(x, y, z - 1).Velocity.Z) / h;
				}
			}
		}
		EnforceInflows(tree);
		EnforceBoundary(tree);
	}
}

void WaterEngine::AdvectDensity(VoxelTree & tree) {
	for (int x = 0; x < tree.Width; x++) {
		for (int y = 0; y < tree.Width; y++) {
			for (int z = 0; z < tree.Height; z++) {
				auto & vox = tree.GetValue(x, y, z);
				Vector3F BackTrace = vox.Velocity * DeltaTime;
				Vector3F PrevPosition = Vector3F(x, y, z) - BackTrace;
				Vector3F ClampedPrev = PrevPosition;
				ClampedPrev.Clamp(Vector3F(0.5, 0.5, 0.5), Vector3F(tree.Width - 0.5, tree.Width - 0.5, tree.Height - 0.5));
				Vector3F PrevRoot = Vector3F(floor(ClampedPrev.X - 0.5), floor(ClampedPrev.Y - 0.5), floor(ClampedPrev.Z - 0.5));
				Vector3F Vd = PrevPosition - PrevRoot;
				float c00 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y, (int)PrevRoot.Z).Density * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y, (int)PrevRoot.Z).Density * Vd.X;
				float c01 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y, (int)PrevRoot.Z + 1).Density * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y, (int)PrevRoot.Z + 1).Density * Vd.X;
				float c10 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y + 1, (int)PrevRoot.Z).Density * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y + 1, (int)PrevRoot.Z).Density * Vd.X;
				float c11 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y + 1, (int)PrevRoot.Z + 1).Density * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y + 1, (int)PrevRoot.Z + 1).Density * Vd.X;
				float c0 = c00 * (1 - Vd.Y) + c10 * Vd.Y;
				float c1 = c01 * (1 - Vd.Y) + c11 * Vd.Y;
				//Could do some linear interp but cba
				tree.GetValueNew(x, y, z).Density = c0 * (1 - Vd.Z) + c1 * Vd.Z;
			}
		}
	}
	EnforceInflows(tree);
	EnforceBoundary(tree);
}

void WaterEngine::AdvectVelocity(VoxelTree & tree) {
	for (int x = 0; x < tree.Width; x++) {
		for (int y = 0; y < tree.Width; y++) {
			for (int z = 0; z < tree.Height; z++) {
				auto & vox = tree.GetValue(x, y, z);
				Vector3F BackTrace = vox.Velocity * DeltaTime;
				Vector3F PrevPosition = Vector3F(x, y, z) - BackTrace;
				Vector3F ClampedPrev = PrevPosition;
				ClampedPrev.Clamp(Vector3F(0.5, 0.5, 0.5), Vector3F(tree.Width - 0.5, tree.Width - 0.5, tree.Height - 0.5));
				Vector3F PrevRoot = Vector3F(floor(ClampedPrev.X - 0.5), floor(ClampedPrev.Y - 0.5), floor(ClampedPrev.Z - 0.5));
				Vector3F Vd = PrevPosition - PrevRoot;
				Vector3F c00 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y, (int)PrevRoot.Z).Velocity * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y, (int)PrevRoot.Z).Velocity * Vd.X;
				Vector3F c01 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y, (int)PrevRoot.Z + 1).Velocity * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y, (int)PrevRoot.Z + 1).Velocity * Vd.X;
				Vector3F c10 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y + 1, (int)PrevRoot.Z).Velocity * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y + 1, (int)PrevRoot.Z).Velocity * Vd.X;
				Vector3F c11 = tree.GetValueNew((int)PrevRoot.X, (int)PrevRoot.Y + 1, (int)PrevRoot.Z + 1).Velocity * (1 - Vd.X) + tree.GetValueNew((int)PrevRoot.X + 1, (int)PrevRoot.Y + 1, (int)PrevRoot.Z + 1).Velocity * Vd.X;
				Vector3F c0 = c00 * (1 - Vd.Y) + c10 * Vd.Y;
				Vector3F c1 = c01 * (1 - Vd.Y) + c11 * Vd.Y;
				//Could do some linear interp but cba
				tree.GetValueNew(x, y, z).Velocity = c0 * (1 - Vd.Z) + c1 * Vd.Z;
			}
		}
	}
	EnforceInflows(tree);
	EnforceBoundary(tree);
}

void WaterEngine::Update(VoxelTree & tree)
{
	DiffuseVelocity(tree);
	//ProjectVelocity(tree);
	AdvectVelocity(tree);
	//ProjectVelocity(tree);
	DiffuseDensity(tree);
	AdvectDensity(tree);
	tree.SwapBuffer();
	//UpdateVelocity();
	////Multi track drifting
	//UpdateHydrostaticStress(tree);
	//UpdateDensity(tree);
	//UpdateSwapBuffer(tree);
}