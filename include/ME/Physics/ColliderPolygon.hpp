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
		
		std::vector<sf::Vector2f> m_edges;

	public:

		const std::vector<sf::Vector2f>& getPoints() const;

		const sf::Vector2f& getPosition() const;

		/// Calculate the area of the polygon.
		float getArea();
		/// Move the origin of the polygon to its centroid (center of mass).
		void center();

		virtual void findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const;

		virtual void checkCollision(const ColliderCircle &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderRect &other, CollisionInfo &info) const;
		virtual void checkCollision(const ColliderPolygon &other, CollisionInfo &info) const;

		virtual Graphic* generateGraphic(const sf::Color &color = sf::Color::White) const;

		virtual ICollider* clone() const;

	private:

		/// Input: points as the coordinates of each vertex
		/// Output: the vector from the origin to the first point followed by the edges of the polygon
		static std::vector<sf::Vector2f> pointsToEdges(const std::vector<sf::Vector2f> &points);
		/// PointsToEdges in reverse
		static std::vector<sf::Vector2f> edgesToPoints(const std::vector<sf::Vector2f> &edges);

	public:

		/// Constructor that takes vectors.
		/// If autoCenter, the collider will be centered around its center of mass.
		ColliderPolygon(std::initializer_list<sf::Vector2f> points, bool autoCenter = true);
		/// Constructor that takes a list of coordinates
		ColliderPolygon(std::initializer_list<float> coords, bool autoCenter = true);
		/// Copy constructor
		ColliderPolygon(const ColliderPolygon &copy);
		virtual ~ColliderPolygon() {}
	};
}


#endif // COLLIDER_POLYGON_HPP