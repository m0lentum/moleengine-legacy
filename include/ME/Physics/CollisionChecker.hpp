#ifndef COLLISION_CHECKER_HPP
#define COLLISION_CHECKER_HPP

#include "CollisionInfo.hpp"

namespace me
{
	class ColliderCircle;
	class ColliderRect;
	class ColliderPolygon;

	class CollisionChecker
	{
	public:

		// Collision checks for different pairs of colliders.
		static void circleCircle(const ColliderCircle &circle1, const ColliderCircle &circle2, CollisionInfo &info);
		static void circleRect(const ColliderCircle &circle, const ColliderRect &rect, CollisionInfo &info);
		static void circlePoly(const ColliderCircle &circle, const ColliderPolygon &poly, CollisionInfo &info);

		static void rectCircle(const ColliderRect &rect, const ColliderCircle &circle, CollisionInfo &info);
		static void rectRect(const ColliderRect &rect1, const ColliderRect &rect2, CollisionInfo &info);
		static void rectPoly(const ColliderRect &rect, const ColliderPolygon &poly, CollisionInfo &info);

		static void polyCircle(const ColliderPolygon &poly, const ColliderCircle &circle, CollisionInfo &info);
		static void polyRect(const ColliderPolygon &poly, const ColliderRect &rect, CollisionInfo &info);
		static void polyPoly(const ColliderPolygon &poly1, const ColliderPolygon &poly2, CollisionInfo &info);

	private:

		struct PolyAxisInfo {
			float width = 0; // Width when projected to the given axis
			sf::Vector2f point1; // Farthest point along the axis
			bool hasPoint2 = false;
			sf::Vector2f point2; // Other farthest point if two are an equal distance away
		};

		static float EPSILON; // Threshold to determine whether two distances are the same

		/// Distance from the origin of a polygon to the farthest point when projected to an axis.
		static PolyAxisInfo polyWidthOnAxis(const std::vector<sf::Vector2f> &edges, const sf::Vector2f &axis);
		/// Same for a rectangle
		static PolyAxisInfo rectWidthOnAxis(const sf::Vector2f dimensions[2], const sf::Vector2f &axis);
	};
}


#endif // COLLISION_CHECKER_HPP