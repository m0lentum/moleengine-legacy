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
		static sf::Vector2f closestPtCirclePoint(const sf::Vector2f &circlePos, float circleRadius, const sf::Vector2f &point);
		static sf::Vector2f closestPtRectPoint(const sf::Transform &rectTransform, float rectHalfwidth, float rectHalfheight, const sf::Vector2f &point);

	private:

		struct PolyAxisInfo {
			float width = 0; // Width when projected to the given axis
			sf::Vector2f point1; // Farthest point along the axis
			bool hasPoint2 = false;
			sf::Vector2f point2; // Other farthest point if two are an equal distance away
		};

		static float EPSILON; // Tolerance to determine whether two directions are the same

		/// Rotate and scale all vectors by the object's values, but do not move them
		static void transformVectors(std::vector<sf::Vector2f> &vecs, GameObject *obj);
		static void rotateVectors(std::vector<sf::Vector2f> &vecs, float angle);

		/// Distance from the origin of a polygon to the farthest point when projected to an axis.
		static PolyAxisInfo polyWidthOnAxis(const std::vector<sf::Vector2f> &edges, const sf::Vector2f &axis);
		static float rectWidthOnAxis(const sf::Vector2f &hw, const sf::Vector2f &hh, const sf::Vector2f &axis);

		PrimitiveQueries() {} // can't instantiate
	};
}


#endif // COLLISION_CHECKER_HPP