#include <Physics/ColliderCircle.hpp>
#include <Physics/PhysicsObject.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Graphics/Graphic.hpp>

namespace me
{
	const float ColliderCircle::getRadius() const
	{
		// The collider has to be a perfect circle for calculations to work,
		// so only consider one dimension of the containing object's scale and assume the other is the same
		return m_parent->getScale().x * m_radius;
	}

	const sf::Vector2f& ColliderCircle::getPosition() const
	{
		return m_parent->getPosition();
	}


	void ColliderCircle::checkCollision(const ColliderCircle &other, CollisionInfo &info) const
	{
		CollisionChecker::circleCircle(other, *this, info);
	}

	void ColliderCircle::checkCollision(const ColliderRect &other, CollisionInfo &info) const
	{
		CollisionChecker::rectCircle(other, *this, info);
	}

	void ColliderCircle::checkCollision(const ColliderPolygon &other, CollisionInfo &info) const
	{
		CollisionChecker::polyCircle(other, *this, info);
	}

	void ColliderCircle::findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const
	{
		other.checkCollision(*this, info);
	}
	
	Graphic* ColliderCircle::generateGraphic(const sf::Color &color) const
	{
		return Graphic::makeCircle(m_radius, 25, color);
	}


	ICollider* ColliderCircle::clone() const
	{
		return new ColliderCircle(*this);
	}


	ColliderCircle::ColliderCircle(float radius) :
		m_radius(radius)
	{
	}

	ColliderCircle::ColliderCircle(const ColliderCircle &copy) :
		m_radius(copy.m_radius)
	{
	}
}