#ifndef VEC2_HPP
#define VEC2_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>

const float PI = 3.1415927f;

namespace me
{
	/// A two-dimensional float vector with added utility functions.
	class Vec2
	{
	public:
		float x;
		float y;

	public:
		float getLength() const;
		/// Returns a vector with the same direction as this one but a length of 1.
		Vec2 normalized() const;

		/// Dot product of another vector and this one.
		float dot(const Vec2 &other) const;
		/// Project the vector onto the specified axis. The axis is assumed to be unit length.
		Vec2 projectToAxis(const Vec2 &axis) const;
		/// Project the vector onto the specified vector. The target vector does not have to be unit length.
		Vec2 projectToVector(const Vec2 &vector) const;

		/// Rotate the vector by the specified number of radians.
		void rotateRad(float radians);
		/// Rotate the vector by the specified number of degrees.
		void rotateDeg(float degrees);
		/// Left normal of the vector
		Vec2 leftNormal() const;
		/// Right normal of the vector
		Vec2 rightNormal() const;

		Vec2(float x = 0, float y = 0);
		~Vec2();
	};

	/// Negate the vector
	Vec2 operator -(const Vec2 &right);
	/// Add two vectors
	Vec2 operator +(const Vec2 &left, const Vec2 &right);
	/// Subtract vector right from vector left
	Vec2 operator -(const Vec2 &left, const Vec2 &right);
	/// Multiply vector left by constant right
	Vec2 operator *(const Vec2 &left, float right);
	/// Multiply vector right by constant left
	Vec2 operator *(float left, const Vec2 &right);
	/// Divide vector left by constant right
	Vec2 operator /(const Vec2 &left, float right);

	/// Add right to left and assign to left
	Vec2& operator +=(Vec2 &left, const Vec2 &right);
	/// Subtract right from left and assign to left
	Vec2& operator -=(Vec2 &left, const Vec2 &right);
	/// Multiply left by right and assign to left
	Vec2& operator *=(Vec2 &left, float right);
	/// Divide left by right and assign to left
	Vec2& operator /=(Vec2 &left, float right);

	// Use inline definitions, similarly to the SFML implementation
#include "Vec2.inl"
}




#endif //VEC2_HPP