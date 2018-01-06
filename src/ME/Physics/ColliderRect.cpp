#include <Physics/ColliderRect.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Physics/VectorMath.hpp>
#include <cmath>
#include <Graphics/Graphic.hpp>

namespace me
{
	void ColliderRect::checkCollision(const ColliderCircle &other, Contact &info) const
	{
		CollisionChecker::circleRect(other, *this, info);
	}

	void ColliderRect::checkCollision(const ColliderRect &other, Contact &info) const
	{
		CollisionChecker::rectRect(other, *this, info);
	}

	void ColliderRect::checkCollision(const ColliderPolygon &other, Contact &info) const
	{
		CollisionChecker::polyRect(other, *this, info);
	}

	void ColliderRect::findTypeAndCheckCollision(const ICollider &other, Contact &info) const
	{
		other.checkCollision(*this, info);
	}



	sf::VertexArray ColliderRect::toVertexArray(const sf::Color &color) const
	{
		return Graphic::makeRect(m_halfWidth * 2, m_halfHeight * 2, color);
	}


	ColliderRect::ColliderRect(float width, float height) :
		m_halfWidth(width / 2),
		m_halfHeight(height / 2)
	{
	}

	ColliderRect::ColliderRect(const ColliderRect &copy) :
		m_halfWidth(copy.m_halfWidth),
		m_halfHeight(copy.m_halfHeight)
	{
	}

	ColliderRect::ColliderRect(ColliderRect&& move) :
		m_halfWidth(std::move(move.m_halfWidth)),
		m_halfHeight(std::move(move.m_halfHeight))
	{
	}

	ColliderRect& ColliderRect::operator=(ColliderRect&& other)
	{
		if (this != &other)
		{
			m_halfWidth = std::move(other.m_halfWidth);
			m_halfHeight = std::move(other.m_halfHeight);
		}

		return *this;
	}

	ColliderRect::~ColliderRect()
	{
	}
}