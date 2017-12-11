#ifndef ICOLLIDER_HPP
#define ICOLLIDER_HPP

namespace me
{
	class ColliderCircle;
	class ColliderRect;
	class ColliderPolygon;
	struct CollisionInfo;

	class ICollider
	{
	public:

		/// This will resolve the type of the first object similarly to the Visitor pattern.
		/// One of the other collider's checkCollision methods will be called accordingly to the result.
		virtual void findTypeAndCheckCollision(const ICollider &other, CollisionInfo &info) const = 0;

		// Every collider type must implement collisions against every other type.
		// These functions will be called by the other object involved in the collision.
		// Results will be written into the given CollisionInfo object.
		virtual void checkCollision(const ColliderCircle &other, CollisionInfo &info) const = 0;
		virtual void checkCollision(const ColliderRect &other, CollisionInfo &info) const = 0;
		virtual void checkCollision(const ColliderPolygon &other, CollisionInfo &info) const = 0;


		virtual ~ICollider() {}
	};
}


#endif // ICOLLIDER_HPP