#include <Physics/CollisionChecker.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/VectorMath.hpp>
//#include <iostream>

namespace me
{
	void CollisionChecker::checkCollision(const ICollider &obj1, const ICollider &obj2, CollisionInfo &info)
	{
		obj1.findTypeAndCheckCollision(obj2, info);
	}

	//===================  CIRCLE and X  ===================
	void CollisionChecker::circleCircle(const ColliderCircle &obj1, const ColliderCircle &obj2, CollisionInfo &info)
	{
		sf::Vector2f separation = obj2.getPosition() - obj1.getPosition();
		sf::Vector2f axis = VectorMath::normalize(separation);

		float penDepth = obj1.getRadius() + obj2.getRadius() - VectorMath::getLength(separation);
		if (penDepth > 0) // A collision occurred
		{
			info.areColliding = true;
			info.penetration = axis * -penDepth;
			info.point1 = axis * obj1.getRadius();
			info.point2 = axis * -obj2.getRadius();
		}
	}

	void CollisionChecker::circleRect(const ColliderCircle &obj1, const ColliderRect &obj2, CollisionInfo &info)
	{

	}

	void CollisionChecker::circlePoly(const ColliderCircle &obj1, const ColliderPolygon &obj2, CollisionInfo &info)
	{

	}


	//===================  RECT and X  ===================
	void CollisionChecker::rectCircle(const ColliderRect &obj1, const ColliderCircle &obj2, CollisionInfo &info)
	{

	}

	void CollisionChecker::rectRect(const ColliderRect &obj1, const ColliderRect &obj2, CollisionInfo &info)
	{

	}

	void CollisionChecker::rectPoly(const ColliderRect &obj1, const ColliderPolygon &obj2, CollisionInfo &info)
	{

	}


	//===================  POLY and X ===================
	void CollisionChecker::polyCircle(const ColliderPolygon &obj1, const ColliderCircle &obj2, CollisionInfo &info)
	{

	}

	void CollisionChecker::polyRect(const ColliderPolygon &obj1, const ColliderRect &obj2, CollisionInfo &info)
	{

	}

	void CollisionChecker::polyPoly(const ColliderPolygon &obj1, const ColliderPolygon &obj2, CollisionInfo &info)
	{

	}
}