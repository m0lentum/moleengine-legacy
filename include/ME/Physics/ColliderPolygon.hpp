#ifndef COLLIDER_POLYGON_HPP
#define COLLIDER_POLYGON_HPP

#include <vector>
#include <initializer_list>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include "ICollider.hpp"

namespace me
{
	/// Arbitrary CONVEX polygon collider.
	class ColliderPolygon : public ICollider
	{
	private:
		
		std::vector<sf::Vector2f> m_points;
		std::vector<sf::Vector2f> m_normals;

	public:

		std::vector<sf::Vector2f> getPoints() const;
		std::vector<sf::Vector2f> getNormals() const;


		/// Calculate the area of the polygon.
		float getArea();
		/// Move the origin of the polygon to its centroid (center of mass).
		void center();


		virtual void findTypeAndCheckCollision(const ICollider &other, Contact &info) const;

		virtual void checkCollision(const ColliderCircle &other, Contact &info) const;
		virtual void checkCollision(const ColliderRect &other, Contact &info) const;
		virtual void checkCollision(const ColliderPolygon &other, Contact &info) const;


		sf::VertexArray toVertexArray(const sf::Color &color = sf::Color::White) const;

	private:

		/// Input: points as the coordinates of each vertex
		/// Output: the vector from the origin to the first point followed by the edges of the polygon
		static std::vector<sf::Vector2f> pointsToEdges(const std::vector<sf::Vector2f> &points);
		/// PointsToEdges in reverse
		static std::vector<sf::Vector2f> edgesToPoints(const std::vector<sf::Vector2f> &edges);

		void calculateNormals();

	public:

		/// Constructor that takes vectors.
		/// If autoCenter, the collider will be centered around its center of mass.
		ColliderPolygon(std::initializer_list<sf::Vector2f> points, bool autoCenter = true);
		/// Constructor that takes a list of coordinates
		ColliderPolygon(std::initializer_list<float> coords, bool autoCenter = true);

		ColliderPolygon(const ColliderPolygon &copy);
		virtual ~ColliderPolygon() {}
	};
}


#endif // COLLIDER_POLYGON_HPP