#pragma once
#include <cmath>
struct alignas(8) Vector {
	float X = 0, Y = 0;//x y z
	//Vector(float x = 0, double y = 0) : X(x), Y(y) {};

//	Vector(const Vector&) = default;
//	Vector(Vector&&) = default;
/*	Vector& operator=(const Vector& v)
	{
		X = v.X;
		Y = v.Y;
		return *this;
	}*/
//	Vector& operator=(Vector&&) = default;
//	~Vector() = default;
	Vector operator+(Vector vec) {
		return Vector{X + vec.X, Y + vec.Y};
	}
	Vector operator-(Vector vec) {
		return Vector{X - vec.X, Y - vec.Y};
	}
	Vector operator-(float v) {
		return Vector{X - v, Y - v};
	}
	void operator-=(Vector vec) {
		X -= vec.X;
		Y -= vec.Y;
	}
	void operator+=(Vector vec) {
		X += vec.X;
		Y += vec.Y;
	}
	void operator*=(float v) {
		X *= v;
		Y *= v;
	}
	void operator/=(float v) {
		X /= v;
		Y /= v;
	}
	float Dot(Vector vec) {
		return (X * vec.X) + (Y * vec.Y);
	}
	float Magnitude()
	{
		return std::sqrt(Dot(*this));
	}
	Vector operator/(float v) {
		return Vector{X / v, Y / v};
	}
	Vector operator*(float v) {
		return Vector{X * v, Y * v};
	}
	Vector Abs(){
		return Vector{abs(X), abs(Y)};
	}
	void Clamp(Vector min, Vector max) {
		X = std::min(std::max(X, min.X), max.X);
		Y = std::min(std::max(Y, min.Y), max.Y);
	}
	friend std::ostream& operator<<(std::ostream& os, const Vector& p)
	{
		os << p.X  << ":" << p.Y;
		return os;
	}
};

