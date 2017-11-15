#include <Physics/ColliderPolygon.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Physics/VectorMath.hpp>
#include <cmath>
#include <Graphics/Graphic.hpp>
#include <iostream>

namespace me
{
	const sf::Vector2f& ColliderPolygon::getPosition() const
	{
		return m_parent->getPosition();
	}

	void ColliderPolygon::checkCollision(const ColliderCircle &other, CollisionInfo &info) const
	{
		CollisionChecker::circlePoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderRect &other, CollisionInfo &info) const
	{
		CollisionChecker::rectPoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderPolygon &other, CollisionInfo &info) const
	{
		CollisionChecker::polyPoly(other, *this, info);
	}

	void ColliderPolygon::findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const
	{
		other.checkCollision(*this, info);
	}

	Graphic* ColliderPolygon::generateGraphic(const sf::Color &color) const
	{
		return Graphic::makePolygon(m_points, color);
	}


	ICollider* ColliderPolygon::clone() const
	{
		return new ColliderPolygon(*this);
	}


	ColliderPolygon::ColliderPolygon(std::initializer_list<sf::Vector2f> points) :
		m_points(points)
	{
		for (const auto &point : m_points) {
			std::cout << point.x << ", " << point.y << ";";
		}
		std::cout << std::endl;
	}

	ColliderPolygon::ColliderPolygon(std::initializer_list<float> coords)
	{
		auto end = coords.end();
		if (coords.size() % 2 != 0)
		{
			end--; // Ignore the last argument if the number of coordinates given is odd
			std::cerr << "Warning: odd number of coordinates given to ColliderPolygon." << std::endl;
		}
		for (auto i = coords.begin(); i != end; i += 2) {
			m_points.push_back(sf::Vector2f(*i, *(i + 1)));
		}

		for (const auto &point : m_points) {
			std::cout << point.x << ", " << point.y << ";";
		}
		std::cout << std::endl;
	}

	ColliderPolygon::ColliderPolygon(const ColliderPolygon &copy)
	{
	}
}