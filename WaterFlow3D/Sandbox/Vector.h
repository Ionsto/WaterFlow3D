#pragma once
#include <cmath>
struct Vector {
	double X = 0, Y = 0,Z = 0;//x y z
	Vector(double x = 0, float y = 0,float z = 0) : X(x), Y(y), Z(z) {};

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
	Vector operator-(double v) {
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
	void operator*=(double v) {
		X *= v;
		Y *= v;
		Z *= v;
	}
	void operator/=(double v) {
		X /= v;
		Y /= v;
		Z /= v;
	}
	double Dot(Vector vec) {
		return (X * vec.X) + (Y * vec.Y);
	}
	double Magnitude()
	{
		return std::sqrt(Dot(*this));
	}
	Vector operator/(double v) {
		return Vector(X / v, Y / v, Z / v);
	}
	Vector operator*(double v) {
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
};

