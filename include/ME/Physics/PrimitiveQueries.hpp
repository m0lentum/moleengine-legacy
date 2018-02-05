#ifndef COLLISION_CHECKER_HPP
#define COLLISION_CHECKER_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace me
{
	class ColliderCircle;
	class ColliderRect;
	class ColliderPolygon;
	class ICollider;
	struct Contact;
	class GameObject;

	struct EdgeEdgeIntersection
	{
		// When two edges are parallel we need two contact points
		bool doesIntersect = false;
		sf::Vector2f penetration;
		sf::Vector2f point1;
		sf::Vector2f point2;
	};

	class PrimitiveQueries
	{
	public:

		/// Checks for collision between two colliders.
		static void checkCollision(const ICollider &coll1, const ICollider &coll2, Contact &info);

		// Collision checks for different pairs of colliders.
		static void circleCircle(const ColliderCircle &circle1, const ColliderCircle &circle2, Contact &info);
		static void circleRect(const ColliderCircle &circle, const ColliderRect &rect, Contact &info);
		static void circlePoly(const ColliderCircle &circle, const ColliderPolygon &poly, Contact &info);

		static void rectCircle(const ColliderRect &rect, const ColliderCircle &circle, Contact &info);
		static void rectRect(const ColliderRect &rect1, const ColliderRect &rect2, Contact &info);
		static void rectPoly(const ColliderRect &rect, const ColliderPolygon &poly, Contact &info);

		static void polyCircle(const ColliderPolygon &poly, const ColliderCircle &circle, Contact &info);
		static void polyRect(const ColliderPolygon &poly, const ColliderRect &rect, Contact &info);
		static void polyPoly(const ColliderPolygon &poly1, const ColliderPolygon &poly2, Contact &info);

		// Closest-point queries
		static sf::Vector2f closestPtOnCircleToPoint(const sf::Vector2f &circlePos, float circleRadius, const sf::Vector2f &point);
		static sf::Vector2f closestPtOnRectToPoint(const sf::Transform &rectTransform, float rectHalfwidth, float rectHalfheight, const sf::Vector2f &point);

		// AABB queries

		/// Returns whether or not two AABBs intersect and if so, the two points where they do, projected onto the surface of the first box
		static EdgeEdgeIntersection intersectAABBs(const sf::Vector2f &pos1, float hw1, float hh1, const sf::Vector2f &pos2, float hw2, float hh2);

	private:

		static float EPSILON; // Tolerance to determine whether two directions are the same

		/// Rotate and scale all vectors by the object's values, but do not move them
		static void transformPolyPoints(std::vector<sf::Vector2f> &vecs, GameObject *obj);
		static void rotatePolyNormals(std::vector<sf::Vector2f> &vecs, float angle);

		/// Distance from the origin of a polygon to the farthest point when projected to an axis.
		static float polyHalfwidthOnAxis(const std::vector<sf::Vector2f> &points, const sf::Vector2f &axis);
		static float rectHalfwidthOnAxis(const sf::Vector2f &hw, const sf::Vector2f &hh, const sf::Vector2f &axis);
		/// Returns the index of the normal nearest to opposite of the axis direction
		static int findOppositePolyEdge(const std::vector<sf::Vector2f> &normals, const sf::Vector2f &axis);
		/// Returns the index of the polygon endpoint (not necessarily closest point on the polygon) closest to the given point
		static int findClosestPolyPoint(const std::vector<sf::Vector2f> &points, const sf::Vector2f &target);

		/// Helper function to treat two parallel OBBs as AABBs
		static EdgeEdgeIntersection intersectParallelRects(const sf::Transform &transform, const sf::Vector2f &pos1, const sf::Vector2f &pos2, float hw1, float hw2, float hw3, float hw4);

		/// Finds the overlapping segement of two parallel edges, projected onto the first. Assumes the edges run in opposing directions.
		static void findOverlapOfEdges(const sf::Vector2f points[4], sf::Vector2f &out1, sf::Vector2f &out2);

		PrimitiveQueries() {} // can't instantiate
	};
}


#endif // COLLISION_CHECKER_HPP