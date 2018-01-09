#ifndef VEC2MATH_HPP
#define VEC2MATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace me
{
	/// Collection of math functions for handling two-dimensional float vectors.
	namespace VectorMath
	{
		const float PI = 3.1415927f;

		float getLength(const sf::Vector2f &vector);
		float getLengthSquared(const sf::Vector2f &vector);
		/// Returns a vector with the same direction as this one but a length of 1.
		sf::Vector2f normalize(const sf::Vector2f &vector);
		/// std::abs both coordinates
		sf::Vector2f abs(const sf::Vector2f &vector);

		/// Dot product of two vectors.
		float dot(const sf::Vector2f &v1, const sf::Vector2f &v2);
		/// Project the vector onto the specified axis. The axis is assumed to be unit length.
		sf::Vector2f projectToAxis(const sf::Vector2f &vector, const sf::Vector2f &axis);
		/// Project the vector onto the specified vector. The target vector does not have to be unit length.
		sf::Vector2f projectToVector(const sf::Vector2f &vector, const sf::Vector2f &target);

		/// Rotate the vector by the specified number of radians.
		sf::Vector2f rotateRad(const sf::Vector2f &vector, float radians);
		/// Rotate the vector by the specified number of degrees.
		sf::Vector2f rotateDeg(const sf::Vector2f &vector, float degrees);
		/// Left normal of the vector
		sf::Vector2f leftNormal(const sf::Vector2f &vector);
		/// Right normal of the vector
		sf::Vector2f rightNormal(const sf::Vector2f &vector);

		/// Turn radians to degrees.
		float radToDeg(float radians);
		/// Turn degrees to radians.
		float degToRad(float degrees);
	}
}

#endif //VEC2MATH_HPP