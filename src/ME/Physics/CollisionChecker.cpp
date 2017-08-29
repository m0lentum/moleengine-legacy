#include <Physics/CollisionChecker.hpp>
#include <iostream>

namespace me
{
	void CollisionChecker::checkCollision(const ICollider &obj1, const ICollider &obj2, CollisionInfo &info)
	{
		obj1.findTypeAndCheckCollision(obj2, info);
	}

	//===================  CIRCLE and X  ===================
	void CollisionChecker::circleCircle(const ColliderCircle &obj1, const ColliderCircle &obj2, CollisionInfo &info)
	{
		// this is just for testing to make sure the right function gets called
		std::cout << "Checking circle-circle collision!" << std::endl;
		info.point = sf::Vector2f(10, 10);
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