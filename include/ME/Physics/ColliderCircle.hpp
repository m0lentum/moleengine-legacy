#ifndef COLLIDER_CIRCLE_HPP
#define COLLIDER_CIRCLE_HPP

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include "ICollider.hpp"

namespace me
{
	class ColliderCircle : public ICollider
	{
	private:

		float m_radius;

	public:

		const float getRadius() const;


		virtual void findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const;

		virtual void checkCollision(const ColliderCircle &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderRect &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderPolygon &other, CollisionInfo &info) const;


		sf::VertexArray toVertexArray(const sf::Color &color = sf::Color::White) const;


		ColliderCircle(float radius);
		ColliderCircle(const ColliderCircle &copy);
		ColliderCircle(ColliderCircle&& move);
		ColliderCircle& operator=(ColliderCircle&& other);
		virtual ~ColliderCircle();
	};
}

#endif // COLLIDER_CIRCLE_HPP