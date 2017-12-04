#include <Physics/ColliderRect.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Physics/VectorMath.hpp>
#include <cmath>
#include <Graphics/Graphic.hpp>

namespace me
{
	const sf::Vector2f ColliderRect::getWidthAxis() const
	{
		float rot = VectorMath::degToRad(m_parent->getRotation());
		return sf::Vector2f(std::cos(rot), std::sin(rot));
	}

	const sf::Vector2f& ColliderRect::getPosition() const
	{
		return m_parent->getPosition();
	}

	void ColliderRect::checkCollision(const ColliderCircle &other, CollisionInfo &info) const
	{
		CollisionChecker::circleRect(other, *this, info);
	}

	void ColliderRect::checkCollision(const ColliderRect &other, CollisionInfo &info) const
	{
		CollisionChecker::rectRect(other, *this, info);
	}

	void ColliderRect::checkCollision(const ColliderPolygon &other, CollisionInfo &info) const
	{
		CollisionChecker::polyRect(other, *this, info);
	}

	void ColliderRect::findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const
	{
		other.checkCollision(*this, info);
	}

	Graphic* ColliderRect::generateGraphic(const sf::Color &color) const
	{
		return new Graphic(Graphic::makeRect(m_halfWidth * 2, m_halfHeight * 2, color));
	}


	ICollider* ColliderRect::clone() const
	{
		return new ColliderRect(*this);
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
}