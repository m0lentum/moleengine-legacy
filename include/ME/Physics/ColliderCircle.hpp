#ifndef COLLIDER_CIRCLE_HPP
#define COLLIDER_CIRCLE_HPP

#include "ICollider.hpp"

namespace me
{
	class ColliderCircle : public ICollider
	{
	private:
		const float m_radius;

	public:
		const float getRadius() const;

		const sf::Vector2f& getPosition() const;

		virtual void findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const;

		virtual void checkCollision(const ColliderCircle &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderRect &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderPolygon &other, CollisionInfo &info) const;

		virtual Graphic* generateGraphic(const sf::Color &color = sf::Color::White) const;
		
		virtual ICollider* clone() const;

		ColliderCircle(float radius);
		ColliderCircle(const ColliderCircle &copy);
		virtual ~ColliderCircle() {}
	};
}

#endif // COLLIDER_CIRCLE_HPP