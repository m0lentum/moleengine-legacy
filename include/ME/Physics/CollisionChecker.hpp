#ifndef COLLISION_CHECKER_HPP
#define COLLISION_CHECKER_HPP

#include "CollisionInfo.hpp"
#include "PhysicsObject.hpp"

namespace me
{
	class ColliderCircle;
	class ColliderRect;
	class ColliderPolygon;

	class CollisionChecker
	{
	public:
		/// Checks for collision between obj1 and obj2.
		static void checkCollision(const ICollider &obj1, const ICollider &obj2, CollisionInfo &info);

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
		/// Distance from the origin of a polygon to the farthest point when projected to an axis.
		static float polyWidthOnAxis(const std::vector<sf::Vector2f> &edges, const sf::Vector2f &axis);
	};
}


#endif // COLLISION_CHECKER_HPP