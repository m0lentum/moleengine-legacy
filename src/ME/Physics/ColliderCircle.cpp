#include <Physics/ColliderCircle.hpp>
#include <Physics/PrimitiveQueries.hpp>
#include <Graphics/Graphic.hpp>

namespace me
{
	void ColliderCircle::checkCollision(const ColliderCircle &other, Contact &info) const
	{
		PrimitiveQueries::circleCircle(other, *this, info);
	}

	void ColliderCircle::checkCollision(const ColliderRect &other, Contact &info) const
	{
		PrimitiveQueries::rectCircle(other, *this, info);
	}

	void ColliderCircle::checkCollision(const ColliderPolygon &other, Contact &info) const
	{
		PrimitiveQueries::polyCircle(other, *this, info);
	}

	void ColliderCircle::findTypeAndCheckCollision(const ICollider &other, Contact &info) const
	{
		other.checkCollision(*this, info);
	}



	const float ColliderCircle::getRadius() const
	{
		return m_radius;
	}

	
	sf::VertexArray ColliderCircle::toVertexArray(const sf::Color &color) const
	{
		return Graphic::makeCircle(m_radius, 25, color);
	}


	ColliderCircle::ColliderCircle(float radius) :
		m_radius(radius)
	{
	}

	ColliderCircle::ColliderCircle(const ColliderCircle &copy) :
		m_radius(copy.m_radius)
	{
		linkRigidBody(copy.m_rigidBody);
	}

	ColliderCircle::ColliderCircle(ColliderCircle&& move) :
		m_radius(std::move(move.m_radius))
	{
		linkRigidBody(move.m_rigidBody);
	}

	ColliderCircle& ColliderCircle::operator=(ColliderCircle&& other)
	{
		if (this != &other)
		{
			m_radius = std::move(other.m_radius);
			linkRigidBody(other.m_rigidBody);
		}

		return *this;
	}

	ColliderCircle::~ColliderCircle()
	{
	}
}