#ifndef COLLIDER_RECT_HPP
#define COLLIDER_RECT_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
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


		virtual void findTypeAndCheckCollision(const ICollider &other, Contact &info) const;

		virtual void checkCollision(const ColliderCircle &other, Contact &info) const;
		virtual void checkCollision(const ColliderRect &other, Contact &info) const;
		virtual void checkCollision(const ColliderPolygon &other, Contact &info) const;


		sf::VertexArray toVertexArray(const sf::Color &color = sf::Color::White) const;


		ColliderRect(float width, float height);
		ColliderRect(const ColliderRect &copy);
		ColliderRect(ColliderRect&& move);
		ColliderRect& operator=(ColliderRect&& other);
		virtual ~ColliderRect();
	};
}


#endif // COLLIDER_RECT_HPP