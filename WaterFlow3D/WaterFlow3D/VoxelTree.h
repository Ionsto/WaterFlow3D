#pragma once
#include <array>
struct Vector3F {
	float X = 0, Y = 0, Z = 0;//x y z
	Vector3F(float x, float y, float z) : X(x), Y(y), Z(z) {};
	Vector3F operator+(Vector3F vec) {
		return Vector3F(X + vec.X, Y + vec.Y, Z + vec.Z);
	}
	Vector3F operator-(Vector3F vec) {
		return Vector3F(X - vec.X, Y - vec.Y, Z - vec.Z);
	}
	Vector3F operator*(float v) {
		return Vector3F(X + v, Y + v, Z + v);
	}
	void Clamp(Vector3F min,Vector3F max) {
		X = std::min(std::max(X, min.X), max.X);
		Y = std::min(std::max(Y, min.Y), max.Y);
		Z = std::min(std::max(Z, min.Z), max.Z);
	}
};
struct VoxelData {
	char Type = 0;
	float Density = 0;
	float Pressure = 0;
	Vector3F Velocity;
};
class VoxelTree
{
private:
	int SwapBufferIndex = 0;
public:
	static constexpr int Width = 50;
	static constexpr int Height = 1;
	std::array<std::array<VoxelData, (Width+2) * (Width + 2) * (Height + 2)>,2> RawData;
	VoxelTree();
	~VoxelTree();
	VoxelData & GetValue(int x, int y, int z) {
		return RawData[SwapBufferIndex][(z + 1) + ((y + 1) * (Height + 2)) + ((x + 1) * (Height + 2) * (Width + 2))];
	}
	VoxelData & GetValueNew(int x, int y, int z) {
		return RawData[SwapBufferIndex^1][(z + 1) + ((y + 1) * (Height + 2)) + ((x + 1) * (Height + 2) * (Width + 2))];
	}
	void SwapBuffer() {
		SwapBufferIndex ^= 1;
	}
	bool InBounds(int x, int y, int z) {
		return x >= 0 && x < Width && y >= 0 && y < Width &&  z >= 0 && z < Height;
	}
};

