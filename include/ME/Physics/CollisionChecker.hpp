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
		static void circleCircle(const ColliderCircle &obj1, const ColliderCircle &obj2, CollisionInfo &info);
		static void circleRect(const ColliderCircle &obj1, const ColliderRect &obj2, CollisionInfo &info);
		static void circlePoly(const ColliderCircle &obj1, const ColliderPolygon &obj2, CollisionInfo &info);

		static void rectCircle(const ColliderRect &obj1, const ColliderCircle &obj2, CollisionInfo &info);
		static void rectRect(const ColliderRect &obj1, const ColliderRect &obj2, CollisionInfo &info);
		static void rectPoly(const ColliderRect &obj1, const ColliderPolygon &obj2, CollisionInfo &info);

		static void polyCircle(const ColliderPolygon &obj1, const ColliderCircle &obj2, CollisionInfo &info);
		static void polyRect(const ColliderPolygon &obj1, const ColliderRect &obj2, CollisionInfo &info);
		static void polyPoly(const ColliderPolygon &obj1, const ColliderPolygon &obj2, CollisionInfo &info);

	private:
		/// Distance from the origin of a polygon to the farthest point when projected to an axis.
		static float polyWidthOnAxis(const std::vector<sf::Vector2f> &obj1, const sf::Vector2f &axis);
	};
}


#endif // COLLISION_CHECKER_HPP