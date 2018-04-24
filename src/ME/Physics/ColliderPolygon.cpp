#include <Physics/ColliderPolygon.hpp>
#include <Physics/PrimitiveQueries.hpp>
#include <Physics/VectorMath.hpp>
#include <cmath>
#include <Graphics/Graphic.hpp>
#include <cassert>
#include <iostream>

namespace me
{
	void ColliderPolygon::checkCollision(const ColliderCircle &other, Contact &info) const
	{
		PrimitiveQueries::circlePoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderRect &other, Contact &info) const
	{
		PrimitiveQueries::rectPoly(other, *this, info);
	}

	void ColliderPolygon::checkCollision(const ColliderPolygon &other, Contact &info) const
	{
		PrimitiveQueries::polyPoly(other, *this, info);
	}

	void ColliderPolygon::findTypeAndCheckCollision(const ICollider &other, Contact &info) const
	{
		other.checkCollision(*this, info);
	}



	const std::vector<sf::Vector2f>& ColliderPolygon::getPoints() const
	{
		return m_points;
	}

	const std::vector<sf::Vector2f>& ColliderPolygon::getNormals() const
	{
		return m_normals;
	}


	float ColliderPolygon::getArea()
	{
		float area = 0;
		auto last = m_points.size() - 1;
		for (std::vector<sf::Vector2f>::size_type i = 1; i <= last; i++)
		{
			area += m_points[i-1].x * m_points[i].y - m_points[i-1].y * m_points[i].x;
		}
		area += m_points[last].x * m_points[0].y - m_points[last].y * m_points[0].x;

		if (area < 0) area = -area;
		return area / 2;
	}

	void ColliderPolygon::center()
	{
		float areaTimes2 = 0;

		// Calculate the centroid relative to the first point of the polygon
		sf::Vector2f weightedSumOfC;
		auto last = m_points.size() - 1;
		for (std::vector<sf::Vector2f>::size_type i = 1; i <= last; i++)
		{
			sf::Vector2f ctr = m_points[i-1] + m_points[i]; // this is actually 3 times the centroid, but we will do the division later
			float weight = m_points[i-1].x * m_points[i].y - m_points[i-1].y * m_points[i].x;
			areaTimes2 += weight;

			weightedSumOfC += ctr * weight;
		}
		sf::Vector2f ctr = m_points[last] + m_points[0]; // last pair : last to first
		float weight = m_points[last].x * m_points[0].y - m_points[last].y * m_points[0].x;
		areaTimes2 += weight;
		weightedSumOfC += ctr * weight;


		sf::Vector2f centroid = weightedSumOfC / (3 * areaTimes2);

		// move each point so they're placed relative to the centroid
		for (auto &point : m_points) point -= centroid;
	}


	

	sf::VertexArray ColliderPolygon::toVertexArray(const sf::Color &color) const
	{
		return Graphic::makePolygon(m_points, color);
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


	void ColliderPolygon::calculateNormals()
	{
		m_normals.reserve(m_points.size());

		for (std::vector<sf::Vector2f>::size_type i = 1; i < m_points.size(); i++)
		{
			m_normals.push_back(VectorMath::normalize(VectorMath::leftNormal(m_points[i] - m_points[i-1])));
		}
		
		// Last edge
		m_normals.push_back(VectorMath::normalize(VectorMath::leftNormal(m_points[0] - m_points[m_points.size() - 1])));
	}


	ColliderPolygon::ColliderPolygon(std::initializer_list<sf::Vector2f> points, bool autoCenter) :
		m_points(points)
	{
		if (autoCenter) center();

		calculateNormals();
	}

	ColliderPolygon::ColliderPolygon(std::initializer_list<float> coords, bool autoCenter)
	{
		auto end = coords.end();
		if (coords.size() % 2 != 0)
		{
			end--; // Ignore the last argument if the number of coordinates given is odd
			std::cerr << "Warning: odd number of coordinates given to ColliderPolygon." << std::endl;
		}

		m_points.reserve(coords.size() / 2);
		for (auto i = coords.begin(); i != end; i += 2) {
			m_points.push_back(sf::Vector2f(*i, *(i + 1)));
		}

		if (autoCenter) center();

		assert(VectorMath::det(m_points[1] - m_points[0], m_points[2] - m_points[0]) < 0 && "Polygon points must be given in counterclockwise order");

		calculateNormals();
	}

	ColliderPolygon::ColliderPolygon(const ColliderPolygon &copy) :
		m_points(copy.m_points),
		m_normals(copy.m_normals)
	{
		linkRigidBody(copy.m_rigidBody);
	}

	ColliderPolygon::ColliderPolygon(ColliderPolygon &&move) :
		m_points(std::move(move.m_points)),
		m_normals(std::move(move.m_normals))
	{
		linkRigidBody(move.m_rigidBody);
	}

	ColliderPolygon::~ColliderPolygon()
	{
	}

	ColliderPolygon& ColliderPolygon::operator=(ColliderPolygon &&other)
	{
		if (&other != this)
		{
			m_points = std::move(other.m_points);
			m_normals = std::move(other.m_normals);
			m_rigidBody = std::move(other.m_rigidBody);
		}

		return *this;
	}
}