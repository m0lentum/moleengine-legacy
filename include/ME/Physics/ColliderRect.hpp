#ifndef COLLIDER_RECT_HPP
#define COLLIDER_RECT_HPP

#include "ICollider.hpp"

namespace me
{
	class ColliderRect : public ICollider
	{
	private:
		float m_halfWidth;
		float m_halfHeight;

	public:
		inline const float getHalfWidth() const { return m_halfWidth; }
		inline const float getHalfHeight() const { return m_halfHeight; }
		const sf::Vector2f getWidthAxis() const;

		const sf::Vector2f& getPosition() const;

		virtual void findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const;

		virtual void checkCollision(const ColliderCircle &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderRect &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderPolygon &other, CollisionInfo &info) const;

		virtual std::shared_ptr<Graphic> generateGraphic(const sf::Color &color = sf::Color::White) const;

		virtual ICollider* clone() const;

		ColliderRect(float width, float height);
		ColliderRect(const ColliderRect &copy);
		virtual ~ColliderRect() {}
	};
}


#endif // COLLIDER_RECT_HPP