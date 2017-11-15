#ifndef COLLIDER_POLYGON_HPP
#define COLLIDER_POLYGON_HPP

#include "ICollider.hpp"
#include <vector>
#include <initializer_list>

namespace me
{
	/// Arbitrary CONVEX polygon collider.
	class ColliderPolygon : public ICollider
	{
	private:
		
		std::vector<sf::Vector2f> m_points;

	public:

		const sf::Vector2f& getPosition() const;

		virtual void findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const;

		virtual void checkCollision(const ColliderCircle &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderRect &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderPolygon &other, CollisionInfo &info) const;

		virtual Graphic* generateGraphic(const sf::Color &color = sf::Color::White) const;

		virtual ICollider* clone() const;

		/// Constructor that takes vectors
		ColliderPolygon(std::initializer_list<sf::Vector2f> points);
		/// Constructor that takes a list of coordinates
		ColliderPolygon(std::initializer_list<float> coords);
		/// Copy constructor
		ColliderPolygon(const ColliderPolygon &copy);
		virtual ~ColliderPolygon() {}
	};
}


#endif // COLLIDER_POLYGON_HPP