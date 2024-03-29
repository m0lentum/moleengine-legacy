#include <Physics/VectorMath.hpp>

namespace me
{

	float VectorMath::length(const sf::Vector2f &vector)
	{
		return std::sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	float VectorMath::lengthSquared(const sf::Vector2f &vector)
	{
		return vector.x * vector.x + vector.y * vector.y;
	}

	sf::Vector2f VectorMath::normalize(const sf::Vector2f &vector)
	{
		float len = length(vector);
		return sf::Vector2f(vector.x / len, vector.y / len);
	}

	sf::Vector2f VectorMath::abs(const sf::Vector2f &vector)
	{
		return sf::Vector2f(std::abs(vector.x), std::abs(vector.y));
	}

	float VectorMath::dot(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	sf::Vector2f VectorMath::projectToAxis(const sf::Vector2f &vector, const sf::Vector2f &axis)
	{
		float dp = dot(vector, axis);
		return sf::Vector2f(axis.x * dp, axis.y * dp);
	}

	sf::Vector2f VectorMath::projectToVector(const sf::Vector2f &vector, const sf::Vector2f &target)
	{
		// (v dot t / t.length^2) * v
		return projectToAxis(vector, target) / (target.x * target.x + target.y * target.y);
	}

	sf::Vector2f VectorMath::rotateRad(const sf::Vector2f &vector, float radians)
	{
		return sf::Vector2f((std::cos(radians) * vector.x) - (std::sin(radians) * vector.y),
							(std::sin(radians) * vector.x) + (std::cos(radians) * vector.y));
	}

	sf::Vector2f VectorMath::rotateDeg(const sf::Vector2f &vector, float degrees)
	{
		return rotateRad(vector, degToRad(degrees));
	}

	sf::Vector2f VectorMath::leftNormal(const sf::Vector2f &vector)
	{
		return sf::Vector2f(-vector.y, vector.x);
	}

	sf::Vector2f VectorMath::rightNormal(const sf::Vector2f &vector)
	{
		return sf::Vector2f(vector.y, -vector.x);
	}

	float VectorMath::cross2D(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	sf::Vector2f VectorMath::cross2D(const sf::Vector2f &v, float s)
	{
		return sf::Vector2f(s * v.y, -s * v.x);
	}

	sf::Vector2f VectorMath::cross2D(float s, const sf::Vector2f &v)
	{
		return sf::Vector2f(-s * v.y, s * v.x);
	}

	float VectorMath::radToDeg(float radians)
	{
		return radians * 180 / PI;
	}

	float VectorMath::degToRad(float degrees)
	{
		return degrees * PI / 180;
	}
}
