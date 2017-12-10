#ifndef COLLIDER_POLYGON_HPP
#define COLLIDER_POLYGON_HPP

#include <vector>
#include <initializer_list>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>

namespace me
{
	/// Arbitrary CONVEX polygon collider.
	class ColliderPolygon
	{
	private:
		
		std::vector<sf::Vector2f> m_edges;
		std::vector<sf::Vector2f> m_axes;

	public:

		const std::vector<sf::Vector2f>& getEdges() const;
		const std::vector<sf::Vector2f>& getAxes() const;

		const sf::Vector2f& getPosition() const;

		/// Calculate the area of the polygon.
		float getArea();
		/// Move the origin of the polygon to its centroid (center of mass).
		void center();

		sf::VertexArray toVertexArray(const sf::Color &color = sf::Color::White) const;

	private:

		/// Input: points as the coordinates of each vertex
		/// Output: the vector from the origin to the first point followed by the edges of the polygon
		static std::vector<sf::Vector2f> pointsToEdges(const std::vector<sf::Vector2f> &points);
		/// PointsToEdges in reverse
		static std::vector<sf::Vector2f> edgesToPoints(const std::vector<sf::Vector2f> &edges);

		/// Calculate axes from edges
		void calculateAxes();

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