#ifndef COLLIDER_CIRCLE_HPP
#define COLLIDER_CIRCLE_HPP

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>

namespace me
{
	class ColliderCircle
	{
	private:

		float m_radius;

	public:

		const float getRadius() const;

		sf::VertexArray toVertexArray(const sf::Color &color = sf::Color::White) const;


		ColliderCircle(float radius);
		ColliderCircle(const ColliderCircle &copy);
		ColliderCircle(ColliderCircle&& move);
		ColliderCircle& operator=(ColliderCircle&& other);
		virtual ~ColliderCircle();
	};
}

#endif // COLLIDER_CIRCLE_HPP