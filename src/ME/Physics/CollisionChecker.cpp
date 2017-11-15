#include <Physics/CollisionChecker.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/ColliderRect.hpp>
#include <Physics/ColliderPolygon.hpp>
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
		sf::Vector2f distance = obj2.getPosition() - obj1.getPosition();
		sf::Vector2f axis = VectorMath::normalize(distance);

		float penDepth = obj1.getRadius() + obj2.getRadius() - VectorMath::getLength(distance);
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
		sf::Vector2f distance = obj2.getPosition() - obj1.getPosition();

		float dimensions[2][2] = { { obj1.getHalfWidth(), obj1.getHalfHeight() },
			{ obj2.getHalfWidth(), obj2.getHalfHeight() } };

		sf::Vector2f axes[2][2];
		axes[0][0] = obj1.getWidthAxis(); // axes[0] == first object's axes, axes[1] == second object's
		axes[0][1] = VectorMath::leftNormal(axes[0][0]);
		axes[1][0] = obj2.getWidthAxis();
		axes[1][1] = VectorMath::leftNormal(axes[1][0]);


		float penDepth = 1000000;
		int axisOwner = 0;
		int axisIndex = 0;
		bool negateAxis = false;

		for (int i = 0; i < 2; i++) // i == which object we're looking at
		{
			int other = (i + 1) % 2;
			for (int j = 0; j < 2; j++) // j == which axis of the object we're looking at
			{
				float axisDistance = VectorMath::dot(axes[i][j], distance);
				float axisWidthSum = dimensions[i][j]
					+ std::abs(VectorMath::dot(axes[other][0], axes[i][j])) * dimensions[other][0] // project width axis of other object
					+ std::abs(VectorMath::dot(axes[other][1], axes[i][j])) * dimensions[other][1];
				
				float depth = axisWidthSum - std::abs(axisDistance);
				if (depth <= 0)
				{ // no collision on this axis => SAT: no collision at all
					return;
				}
				else if (depth < penDepth)
				{
					penDepth = depth;
					axisOwner = i;
					axisIndex = j;
					negateAxis = axisDistance > 0; // Penetration should always be towards object 1
				}
			}
		}

		info.areColliding = true;
		info.penetration = penDepth * axes[axisOwner][axisIndex];

		if (negateAxis) info.penetration = -info.penetration;

		// TODO: calculate points of impact
	}

	void CollisionChecker::rectPoly(const ColliderRect &obj1, const ColliderPolygon &obj2, CollisionInfo &info)
	{

	}


	//===================  POLY and X  ===================
	void CollisionChecker::polyCircle(const ColliderPolygon &obj1, const ColliderCircle &obj2, CollisionInfo &info)
	{

	}

	void CollisionChecker::polyRect(const ColliderPolygon &obj1, const ColliderRect &obj2, CollisionInfo &info)
	{

	}

	void CollisionChecker::polyPoly(const ColliderPolygon &obj1, const ColliderPolygon &obj2, CollisionInfo &info)
	{
		sf::Vector2f distance = obj2.getPosition() - obj1.getPosition();

		std::vector<sf::Vector2f> edges1 = obj1.getEdges();
		std::vector<sf::Vector2f> edges2 = obj2.getEdges();

		std::vector<sf::Vector2f> axes = obj1.getAxes();
		std::vector<sf::Vector2f> otherAxes = obj2.getAxes();
		axes.insert(axes.end(), otherAxes.begin(), otherAxes.end()); // Put all axes into one vector for easy iteration


		float penDepth = 100000;
		sf::Vector2f penAxis;
		for (auto &axis : axes)
		{
			float distOnAxis = VectorMath::dot(axis, distance);
			if (distOnAxis < 0)
			{
				axis = -axis; // revert the axis if it faces the wrong way
				distOnAxis = -distOnAxis;
			}

			float w1 = polyWidthOnAxis(edges1, axis);
			float w2 = polyWidthOnAxis(edges2, -axis);

			float depth = w1 + w2 - distOnAxis;
			if (depth < 0) // no collision on this axis => SAT: no collision at all
			{
				return;
			}
			else if (depth < penDepth)
			{
				penAxis = axis;
				penDepth = depth;
			}
		}

		info.areColliding = true;
		info.penetration = -penDepth * penAxis;

		// TODO: calculate point(s) of impact
	}

	float CollisionChecker::polyWidthOnAxis(const std::vector<sf::Vector2f> &edges, const sf::Vector2f &axis)
	{
		float max = 0;
		float curr = 0;
		
		for (std::vector<sf::Vector2f>::size_type i = 0; i < edges.size(); i++)
		{
			curr += VectorMath::dot(edges[i], axis);
			if (curr > max) max = curr;
		}

		return max;
	}
}