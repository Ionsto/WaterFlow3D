#pragma once
#include <cmath>
struct Vector {
	float X = 0, Y = 0,Z = 0;//x y z
	Vector(float x = 0, double y = 0,double z = 0) : X(x), Y(y), Z(z) {};

//	Vector(const Vector&) = default;
//	Vector(Vector&&) = default;
	Vector& operator=(const Vector& v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
		return *this;
	}
//	Vector& operator=(Vector&&) = default;
//	~Vector() = default;
	Vector operator+(Vector vec) {
		return Vector(X + vec.X, Y + vec.Y,Z + vec.Z);
	}
	Vector operator-(Vector vec) {
		return Vector(X - vec.X, Y - vec.Y, Z - vec.Z);
	}
	Vector operator-(float v) {
		return Vector(X - v, Y - v, Z - v);
	}
	void operator-=(Vector vec) {
		X -= vec.X;
		Y -= vec.Y;
		Z -= vec.Z;
	}
	void operator+=(Vector vec) {
		X += vec.X;
		Y += vec.Y;
		Z += vec.Z;
	}
	void operator*=(float v) {
		X *= v;
		Y *= v;
		Z *= v;
	}
	void operator/=(float v) {
		X /= v;
		Y /= v;
		Z /= v;
	}
	float Dot(Vector vec) {
		return (X * vec.X) + (Y * vec.Y);
	}
	float Magnitude()
	{
		return std::sqrt(Dot(*this));
	}
	Vector operator/(float v) {
		return Vector(X / v, Y / v, Z / v);
	}
	Vector operator*(float v) {
		return Vector(X * v, Y * v, Z * v);
	}
	Vector Abs(){
		return Vector(abs(X), abs(Y), abs(Z));
	}
	void Clamp(Vector min, Vector max) {
		X = std::min(std::max(X, min.X), max.X);
		Y = std::min(std::max(Y, min.Y), max.Y);
		Z = std::min(std::max(Z, min.Z), max.Z);
	}
	friend std::ostream& operator<<(std::ostream& os, const Vector& p)
	{
		os << p.X  << ":" << p.Y;
		return os;
	}
};

