#include <Physics/ColliderPolygon.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Physics/VectorMath.hpp>
#include <cmath>
#include <Graphics/Graphic.hpp>
#include <iostream>

namespace me
{
	void ColliderPolygon::checkCollision(const ColliderCircle &other, Contact &info) const
	{
		CollisionChecker::circlePoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderRect &other, Contact &info) const
	{
		CollisionChecker::rectPoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderPolygon &other, Contact &info) const
	{
		CollisionChecker::polyPoly(other, *this, info);
	}

	void ColliderPolygon::findTypeAndCheckCollision(const ICollider &other, Contact &info) const
	{
		other.checkCollision(*this, info);
	}



	std::vector<sf::Vector2f> ColliderPolygon::getEdges() const
	{
		return m_edges;
	}

	std::vector<sf::Vector2f> ColliderPolygon::getAxes() const
	{
		return m_axes;
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


	

	sf::VertexArray ColliderPolygon::toVertexArray(const sf::Color &color) const
	{
		return Graphic::makePolygon(edgesToPoints(m_edges), color);
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


	void ColliderPolygon::calculateAxes()
	{
		// Determine which direction the edges are going so we know which way is out. 1 = clockwise, -1 = counter-clockwise
		float direction = VectorMath::dot(VectorMath::leftNormal(m_edges[2]), m_edges[1]) > 0 ? 1.0f : -1.0f;

		sf::Vector2f sum; // The last edge is not stored so we calculate it
		for (std::vector<sf::Vector2f>::size_type i = 1; i < m_edges.size(); i++)
		{
			m_axes.push_back(VectorMath::normalize(VectorMath::leftNormal(m_edges[i]) * direction));

			sum -= m_edges[i]; // negative sum ends up being the vector from the last point to the first
		}
		
		// Last edge
		m_axes.push_back(VectorMath::normalize(VectorMath::leftNormal(sum) * direction));
	}


	ColliderPolygon::ColliderPolygon(std::initializer_list<sf::Vector2f> points, bool autoCenter) :
		m_edges(pointsToEdges(points))
	{
		if (autoCenter) center();

		calculateAxes();
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

		calculateAxes();
	}

	ColliderPolygon::ColliderPolygon(const ColliderPolygon &copy) :
		m_edges(copy.m_edges),
		m_axes(copy.m_axes)
	{
	}
}