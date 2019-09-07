// Vector.hpp
// Records (w, x, y, z) vector with float accuracy

#pragma once

#include "Main.hpp"
#include "File.hpp"

class Vector {
public:
	float x=0.f, y=0.f, z=0.f;
	float& r;
	float& g;
	float& b;

	// constructors
	Vector() : r(x), g(y), b(z) {}
	Vector(const Vector& src) : r(x), g(y), b(z) {
		x = src.x;
		y = src.y;
		z = src.z;
	}
	Vector(const float factor) : r(x), g(y), b(z) {
		x = factor;
		y = factor;
		z = factor;
	}
	Vector(float _x, float _y, float _z) : r(x), g(y), b(z) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vector(float* arr) : r(x), g(y), b(z) {
		x = arr[0];
		y = arr[1];
		z = arr[2];
	}
	~Vector() {}

	// determines whether the vector represents a true volume
	// @return true if all elements are nonzero, false otherwise
	bool hasVolume() const {
		if( x==0.f || y==0.f || z==0.f ) {
			return false;
		}
		return true;
	}

	// calculate dot product of this vector and another
	// @return the dot product of the two vectors
	float dot(const Vector& other) const {
		return x*other.x + y*other.y + z*other.z;
	}

	// calculate cross product of this vector and another
	// @return the cross product of the two vectors
	Vector cross(const Vector& other) const {
		Vector result;
		result.x = y*other.z - z*other.y;
		result.y = z*other.x - x*other.z;
		result.z = x*other.y - y*other.x;
		return result;
	}

	// calculate the length of the vector (uses sqrt)
	// @return the length
	float length() const {
		return sqrtf( x*x + y*y + z*z );
	}

	// calculate the length of the vector, sans sqrt
	// @return the length squared
	float lengthSquared() const {
		return x*x + y*y + z*z;
	}

	// create a normalized copy of this vector
	// @return a normalized version of this vector
	Vector normal() const {
		float l = length();
		return Vector( x/l, y/l, z/l );
	}

	// normalize this vector
	void normalize() {
		float l = length();
		x /= l;
		y /= l;
		z /= l;
	}

	// conversion to glm::vec3
	operator glm::vec3() const {
		return glm::vec3(x,y,z);
	}

	// operator =
	Vector& operator=(const Vector& src) {
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}

	// operator +
	Vector operator+(const Vector& src) const {
		Vector result;
		result.x = x+src.x;
		result.y = y+src.y;
		result.z = z+src.z;
		return result;
	}

	// operator +=
	Vector& operator+=(const Vector& src) {
		x += src.x;
		y += src.y;
		z += src.z;
		return *this;
	}

	// operator -
	Vector operator-(const Vector& src) const {
		Vector result;
		result.x = x-src.x;
		result.y = y-src.y;
		result.z = z-src.z;
		return result;
	}

	// operator -=
	Vector& operator-=(const Vector& src) {
		x -= src.x;
		y -= src.y;
		z -= src.z;
		return *this;
	}

	// operator *
	Vector operator*(const Vector& src) const {
		Vector result;
		result.x = x*src.x;
		result.y = y*src.y;
		result.z = z*src.z;
		return result;
	}
	Vector operator*(const float& src) const {
		Vector result;
		result.x = x*src;
		result.y = y*src;
		result.z = z*src;
		return result;
	}

	// operator *=
	Vector& operator*=(const Vector& src) {
		x *= src.x;
		y *= src.y;
		z *= src.z;
		return *this;
	}
	Vector& operator*=(const float& src) {
		x *= src;
		y *= src;
		z *= src;
		return *this;
	}

	// operator /
	Vector operator/(const Vector& src) const {
		Vector result;
		result.x = x/src.x;
		result.y = y/src.y;
		result.z = z/src.z;
		return result;
	}
	Vector operator/(const float& src) const {
		Vector result;
		result.x = x/src;
		result.y = y/src;
		result.z = z/src;
		return result;
	}

	// operator /=
	Vector& operator/=(const Vector& src) {
		x /= src.x;
		y /= src.y;
		z /= src.z;
		return *this;
	}
	Vector& operator/=(const float& src) {
		x /= src;
		y /= src;
		z /= src;
		return *this;
	}

	// CONDITIONAL OPERATORS

	// operator >
	bool operator>(const Vector& other) const {
		if( lengthSquared() > other.lengthSquared() ) {
			return true;
		} else {
			return false;
		}
	}

	// operator >=
	bool operator>=(const Vector& other) const {
		if( lengthSquared() >= other.lengthSquared() ) {
			return true;
		} else {
			return false;
		}
	}

	// operator ==
	bool operator==(const Vector& other) const {
		if( x==other.x && y==other.y && z==other.z ) {
			return true;
		} else {
			return false;
		}
	}

	// operator <=
	bool operator<=(const Vector& other) const {
		if( lengthSquared() <= other.lengthSquared() ) {
			return true;
		} else {
			return false;
		}
	}

	// operator <
	bool operator<(const Vector& other) const {
		if( lengthSquared() < other.lengthSquared() ) {
			return true;
		} else {
			return false;
		}
	}

	// operator !=
	bool operator!=(const Vector& other) const {
		if( x!=other.x || y!=other.y || z!=other.z ) {
			return true;
		} else {
			return false;
		}
	}

	// save/load this object to a file
	// @param file interface to serialize with
	void serialize(FileInterface * file) {
		file->property("x", x);
		file->property("y", y);
		file->property("z", z);
	}
};