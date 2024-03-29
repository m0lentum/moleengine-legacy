#include <Physics/ColliderRect.hpp>
#include <Physics/PrimitiveQueries.hpp>
#include <Physics/VectorMath.hpp>
#include <cmath>
#include <Graphics/Graphic.hpp>

namespace me
{
	void ColliderRect::checkCollision(const ColliderCircle &other, Contact &info) const
	{
		PrimitiveQueries::circleRect(other, *this, info);
	}

	void ColliderRect::checkCollision(const ColliderRect &other, Contact &info) const
	{
		PrimitiveQueries::rectRect(other, *this, info);
	}

	void ColliderRect::checkCollision(const ColliderPolygon &other, Contact &info) const
	{
		PrimitiveQueries::polyRect(other, *this, info);
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
		linkRigidBody(copy.m_rigidBody);
	}

	ColliderRect::ColliderRect(ColliderRect&& move) :
		m_halfWidth(std::move(move.m_halfWidth)),
		m_halfHeight(std::move(move.m_halfHeight))
	{
		linkRigidBody(move.m_rigidBody);
	}

	ColliderRect& ColliderRect::operator=(ColliderRect&& other)
	{
		if (this != &other)
		{
			m_halfWidth = std::move(other.m_halfWidth);
			m_halfHeight = std::move(other.m_halfHeight);
			m_rigidBody = std::move(other.m_rigidBody);
		}

		return *this;
	}

	ColliderRect::~ColliderRect()
	{
	}
}