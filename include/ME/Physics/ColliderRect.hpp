#ifndef COLLIDER_RECT_HPP
#define COLLIDER_RECT_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>

namespace me
{
	class ColliderRect
	{
	private:

		float m_halfWidth;
		float m_halfHeight;

	public:

		inline const float getHalfWidth() const { return m_halfWidth; }
		inline const float getHalfHeight() const { return m_halfHeight; }


		sf::VertexArray toVertexArray(const sf::Color &color = sf::Color::White) const;


		ColliderRect(float width, float height);
		ColliderRect(const ColliderRect &copy);
		ColliderRect(ColliderRect&& move);
		ColliderRect& operator=(ColliderRect&& other);
		virtual ~ColliderRect();
	};
}


#endif // COLLIDER_RECT_HPP