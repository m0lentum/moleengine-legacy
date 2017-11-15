#include <Physics/ColliderPolygon.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Physics/VectorMath.hpp>
#include <cmath>
#include <Graphics/Graphic.hpp>
#include <iostream>

namespace me
{
	const std::vector<sf::Vector2f>& ColliderPolygon::getPoints() const
	{
		return m_edges;
	}

	const sf::Vector2f& ColliderPolygon::getPosition() const
	{
		return m_parent->getPosition();
	}

	float ColliderPolygon::getArea()
	{
		float getArea = 0;
		sf::Vector2f curr, next; // the sum of edges so far == vector from the first point to the current one
		for (std::vector<sf::Vector2f>::size_type i = 1; i < m_edges.size() - 1; i++)
		{
			curr += m_edges[i];
			next = curr + m_edges[i + 1];

			getArea += std::abs(curr.x * next.y - curr.y * next.x);
		}

		return getArea / 2;
	}

	void ColliderPolygon::center()
	{
		float area = getArea();
		
		// Calculate the centroid relative to the first point of the polygon
		sf::Vector2f weightedSumOfC, curr, next;
		for (std::vector<sf::Vector2f>::size_type i = 1; i < m_edges.size() - 1; i++)
		{
			curr += m_edges[i];
			next = curr + m_edges[i + 1];

			sf::Vector2f ctr = curr + next; // this is actually 3 times the centroid, but we will do the division later
			float weight = std::abs(curr.x * next.y - curr.y * next.x);

			weightedSumOfC += ctr * weight;
		}

		sf::Vector2f centroid = weightedSumOfC / (6 * area);

		m_edges[0] = -centroid; // the first point in the edges represents the centroid
	}




	void ColliderPolygon::checkCollision(const ColliderCircle &other, CollisionInfo &info) const
	{
		CollisionChecker::circlePoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderRect &other, CollisionInfo &info) const
	{
		CollisionChecker::rectPoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderPolygon &other, CollisionInfo &info) const
	{
		CollisionChecker::polyPoly(other, *this, info);
	}

	void ColliderPolygon::findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const
	{
		other.checkCollision(*this, info);
	}

	Graphic* ColliderPolygon::generateGraphic(const sf::Color &color) const
	{
		return Graphic::makePolygon(edgesToPoints(m_edges), color);
	}


	ICollider* ColliderPolygon::clone() const
	{
		return new ColliderPolygon(*this);
	}


	std::vector<sf::Vector2f> ColliderPolygon::pointsToEdges(const std::vector<sf::Vector2f> &points)
	{
		std::vector<sf::Vector2f> edges;

		edges.push_back(points[0]); // the first vector in this format is the vector from the polygon's origin to its first point
		for (std::vector<sf::Vector2f>::size_type i = 1; i < points.size(); i++)
		{
			edges.push_back(points[i] - points[i - 1]); // the edge between the two points
		}

		return edges;
	}

	std::vector<sf::Vector2f> ColliderPolygon::edgesToPoints(const std::vector<sf::Vector2f> &edges)
	{
		std::vector<sf::Vector2f> points;
		
		points.push_back(edges[0]);
		sf::Vector2f sum = edges[0];
		for (std::vector<sf::Vector2f>::size_type i = 1; i < edges.size(); i++)
		{
			sum += edges[i]; // the sum of edges up to this point is the position of the point
			points.push_back(sum);
		}

		return points;
	}



	ColliderPolygon::ColliderPolygon(std::initializer_list<sf::Vector2f> points, bool autoCenter) :
		m_edges(pointsToEdges(points))
	{
		if (autoCenter) center();
	}

	ColliderPolygon::ColliderPolygon(std::initializer_list<float> coords, bool autoCenter)
	{
		auto end = coords.end();
		if (coords.size() % 2 != 0)
		{
			end--; // Ignore the last argument if the number of coordinates given is odd
			std::cerr << "Warning: odd number of coordinates given to ColliderPolygon." << std::endl;
		}

		std::vector<sf::Vector2f> points;
		for (auto i = coords.begin(); i != end; i += 2) {
			points.push_back(sf::Vector2f(*i, *(i + 1)));
		}

		m_edges = pointsToEdges(points);

		if (autoCenter) center();
	}

	ColliderPolygon::ColliderPolygon(const ColliderPolygon &copy)
	{
	}
}