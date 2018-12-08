#pragma once
struct Vector {
	float X = 0, Y = 0;//x y z
	Vector(float x = 0, float y = 0) : X(x), Y(y){};
	Vector operator+(Vector vec) {
		return Vector(X + vec.X, Y + vec.Y);
	}
	Vector operator-(Vector vec) {
		return Vector(X - vec.X, Y - vec.Y);
	}
	Vector operator-(float v) {
		return Vector(X - v, Y - v);
	}
	void operator-=(Vector vec) {
		X -= vec.X;
		Y -= vec.Y;
	}
	void operator+=(Vector vec) {
		X += vec.X;
		Y += vec.Y;
	}
	float Dot(Vector & vec) {
		return X * vec.X + Y * vec.Y;
	}
	Vector operator/(float v) {
		return Vector(X / v, Y / v);
	}
	Vector operator*(float v) {
		return Vector(X * v, Y * v);
	}
	Vector& operator=(const Vector & v) {
		X = v.X;
		Y = v.Y;
		return *this;
	}
	void Clamp(Vector min,Vector max) {
		X = std::min(std::max(X, min.X), max.X);
		Y = std::min(std::max(Y, min.Y), max.Y);
	}
};

