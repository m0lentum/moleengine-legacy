#ifndef VEC2MATH_HPP
#define VEC2MATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>

static const float PI = 3.1415927f;

namespace me
{
	/// Collection of math functions for handling two-dimensional float vectors.
	class VectorMath
	{
	public:
		static float getLength(const sf::Vector2f &vector);
		/// Returns a vector with the same direction as this one but a length of 1.
		static sf::Vector2f normalize(const sf::Vector2f &vector);

		/// Dot product of two vectors.
		static float dot(const sf::Vector2f &v1, const sf::Vector2f &v2);
		/// Project the vector onto the specified axis. The axis is assumed to be unit length.
		static sf::Vector2f projectToAxis(const sf::Vector2f &vector, const sf::Vector2f &axis);
		/// Project the vector onto the specified vector. The target vector does not have to be unit length.
		static sf::Vector2f projectToVector(const sf::Vector2f &vector, const sf::Vector2f &target);

		/// Rotate the vector by the specified number of radians.
		static sf::Vector2f rotateRad(const sf::Vector2f &vector, float radians);
		/// Rotate the vector by the specified number of degrees.
		static sf::Vector2f rotateDeg(const sf::Vector2f &vector, float degrees);
		/// Left normal of the vector
		static sf::Vector2f leftNormal(const sf::Vector2f &vector);
		/// Right normal of the vector
		static sf::Vector2f rightNormal(const sf::Vector2f &vector);

		/// Turn radians to degrees.
		static float radToDeg(float radians);
		/// Turn degrees to radians.
		static float degToRad(float degrees);
	};
}

#endif //VEC2MATH_HPP