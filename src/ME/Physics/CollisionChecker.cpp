#include <Physics/CollisionChecker.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/ColliderRect.hpp>
#include <Physics/ColliderPolygon.hpp>
#include <Physics/ICollider.hpp>
#include <Physics/Contact.hpp>
#include <Physics/VectorMath.hpp>
#include <GameObject.hpp>
#include <iostream>

namespace me
{
	float CollisionChecker::EPSILON = 0.001f;


	void CollisionChecker::checkCollision(const ICollider &coll1, const ICollider &coll2, Contact &info)
	{
		coll1.findTypeAndCheckCollision(coll2, info);
	}

	//===================  CIRCLE and X  ===================
	void CollisionChecker::circleCircle(const ColliderCircle &circle1, const ColliderCircle &circle2, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();
		sf::Vector2f axis = VectorMath::normalize(distance);

		float penDepth = circle1.getRadius() + circle2.getRadius() - VectorMath::getLength(distance);
		if (penDepth > 0) // A collision occurred
		{
			info.areColliding = true;
			info.penetration = axis * -penDepth;
			info.point1 = axis * circle1.getRadius();
			info.point2 = axis * -circle2.getRadius();
		}
	}

	void CollisionChecker::circleRect(const ColliderCircle &circle, const ColliderRect &rect, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		rectCircle(rect, circle, info);
		std::swap(info.obj1, info.obj2);
		info.penetration = -info.penetration;
	}

	void CollisionChecker::circlePoly(const ColliderCircle &circle, const ColliderPolygon &poly, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		polyCircle(poly, circle, info);
		std::swap(info.obj1, info.obj2);
		info.penetration = -info.penetration;
	}


	//===================  RECT and X  ===================
	void CollisionChecker::rectCircle(const ColliderRect &rect, const ColliderCircle &circle, Contact &info)
	{
		const sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		float angle = VectorMath::degToRad(info.obj1->getRotation());
		sf::Vector2f rectWAxis(std::cos(angle), std::sin(angle));
		sf::Vector2f rectHAxis = VectorMath::leftNormal(rectWAxis);

		sf::Vector2f rectDimensions[2] = { rectWAxis * rect.getHalfWidth(), rectHAxis * rect.getHalfHeight() };
		float radius = circle.getRadius();

		sf::Vector2f closestPoint = info.obj1->getPosition() + rectWidthOnAxis(rectDimensions, distance).point1;
		sf::Vector2f toCircleAxis = VectorMath::normalize(info.obj2->getPosition() - closestPoint);

		sf::Vector2f axes[3] = { rectWAxis, rectHAxis, toCircleAxis };
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

			PolyAxisInfo rectW = rectWidthOnAxis(rectDimensions, axis);

			float depth = rectW.width + radius - distOnAxis;
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
	}

	void CollisionChecker::rectRect(const ColliderRect &rect1, const ColliderRect &rect2, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		float dimensions[2][2] = { { rect1.getHalfWidth() * info.obj1->getScale().x, rect1.getHalfHeight() * info.obj1->getScale().y },
			{ rect2.getHalfWidth() * info.obj2->getScale().x, rect2.getHalfHeight() * info.obj2->getScale().y } };

		sf::Vector2f axes[2][2];
		float angle1 = VectorMath::degToRad(info.obj1->getRotation());
		axes[0][0] = sf::Vector2f(std::cos(angle1), std::sin(angle1)); // axes[0] == first rect's axes, axes[1] == second rect's
		axes[0][1] = VectorMath::leftNormal(axes[0][0]);
		float angle2 = VectorMath::degToRad(info.obj2->getRotation());
		axes[1][0] = sf::Vector2f(std::cos(angle2), std::sin(angle2));
		axes[1][1] = VectorMath::leftNormal(axes[1][0]);


		float penDepth = 1000000;
		int axisOwner = 0;
		int axisIndex = 0;
		bool negateAxis = false;

		for (int i = 0; i < 2; i++) // i == which rect we're looking at
		{
			int other = (i + 1) % 2;
			for (int j = 0; j < 2; j++) // j == which axis of the rect we're looking at
			{
				float axisDistance = VectorMath::dot(axes[i][j], distance);
				float axisWidthSum = dimensions[i][j]
					+ std::abs(VectorMath::dot(axes[other][0], axes[i][j])) * dimensions[other][0] // project width axis of other rect
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
					negateAxis = axisDistance > 0; // Penetration should always be towards rectect 1
				}
			}
		}

		info.areColliding = true;
		info.penetration = penDepth * axes[axisOwner][axisIndex];

		if (negateAxis) info.penetration = -info.penetration;

		// TODO: calculate points of impact
	}

	void CollisionChecker::rectPoly(const ColliderRect &rect, const ColliderPolygon &poly, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		polyRect(poly, rect, info);
		std::swap(info.obj1, info.obj2);
		info.penetration = -info.penetration;
	}


	//===================  POLY and X  ===================
	void CollisionChecker::polyCircle(const ColliderPolygon &poly, const ColliderCircle &circle, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();
		
		std::vector<sf::Vector2f> edges = poly.getEdges();
		transformVectors(edges, info.obj1);
		std::vector<sf::Vector2f> axes = poly.getAxes();
		rotateVectors(axes, info.obj1->getRotation());
		float radius = circle.getRadius();

		// With the circle we also need to check the axis between its center and the closest point to it
		sf::Vector2f closestPoint = info.obj1->getPosition() + polyWidthOnAxis(edges, distance).point1;
		sf::Vector2f toCircle = info.obj2->getPosition() - closestPoint;
		axes.push_back(VectorMath::normalize(toCircle));

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

			PolyAxisInfo polyW = polyWidthOnAxis(edges, axis);

			float depth = polyW.width + radius - distOnAxis;
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
		info.penetration = -penAxis * penDepth;
	}

	void CollisionChecker::polyRect(const ColliderPolygon &poly, const ColliderRect &rect, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		std::vector<sf::Vector2f> edges = poly.getEdges();
		transformVectors(edges, info.obj1);
		std::vector<sf::Vector2f> axes = poly.getAxes();
		rotateVectors(axes, info.obj1->getRotation());

		float angle = VectorMath::degToRad(info.obj2->getRotation());
		sf::Vector2f rectWAxis = sf::Vector2f(std::cos(angle), std::sin(angle));
		sf::Vector2f rectHAxis = VectorMath::leftNormal(rectWAxis);
		axes.push_back(rectWAxis);
		axes.push_back(rectHAxis);

		sf::Vector2f rectDimensions[2] = { rectWAxis * rect.getHalfWidth(), rectHAxis * rect.getHalfHeight() };

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

			PolyAxisInfo w1 = polyWidthOnAxis(edges, axis);
			PolyAxisInfo w2 = rectWidthOnAxis(rectDimensions, axis);

			float depth = w1.width + w2.width - distOnAxis;
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

	}

	void CollisionChecker::polyPoly(const ColliderPolygon &poly1, const ColliderPolygon &poly2, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		std::vector<sf::Vector2f> edges1 = poly1.getEdges();
		std::vector<sf::Vector2f> edges2 = poly2.getEdges();
		transformVectors(edges1, info.obj1);
		transformVectors(edges2, info.obj2);

		std::vector<sf::Vector2f> axes = poly1.getAxes();
		std::vector<sf::Vector2f> otherAxes = poly2.getAxes();
		rotateVectors(axes, info.obj1->getRotation());
		rotateVectors(otherAxes, info.obj2->getRotation());
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

			PolyAxisInfo w1 = polyWidthOnAxis(edges1, axis);
			PolyAxisInfo w2 = polyWidthOnAxis(edges2, -axis);

			float depth = w1.width + w2.width - distOnAxis;
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
	}


	void CollisionChecker::transformVectors(std::vector<sf::Vector2f> &vecs, GameObject *obj)
	{
		for (auto &vec : vecs)
		{
			vec = obj->getTransform() * vec - obj->getPosition();
		}
	}

	void CollisionChecker::rotateVectors(std::vector<sf::Vector2f> &vecs, float angle)
	{
		for (auto &vec : vecs)
		{
			vec = VectorMath::rotateDeg(vec, angle);
		}
	}

	CollisionChecker::PolyAxisInfo CollisionChecker::polyWidthOnAxis(const std::vector<sf::Vector2f> &edges, const sf::Vector2f &axis)
	{
		PolyAxisInfo info;

		sf::Vector2f curr;
		
		for (std::vector<sf::Vector2f>::size_type i = 0; i < edges.size(); i++)
		{
			curr += edges[i];
			float projLength = VectorMath::dot(curr, axis);
			
			if (std::abs(projLength - info.width) < EPSILON) { // This edge is perpendicular to the axis, make both ends of the edge the farthest points
				info.width = projLength;
				info.hasPoint2 = true;
				info.point1 = curr - edges[i];
				info.point2 = curr;
			}
			else if (projLength > info.width)
			{
				info.width = projLength;
				info.hasPoint2 = false;
				info.point1 = curr;
			}
		}

		return info;
	}

	CollisionChecker::PolyAxisInfo CollisionChecker::rectWidthOnAxis(const sf::Vector2f dimensions[2], const sf::Vector2f &axis)
	{
		PolyAxisInfo info;

		float wLength = VectorMath::dot(dimensions[0], axis);
		float hLength = VectorMath::dot(dimensions[1], axis);

		info.width = std::abs(wLength) + std::abs(hLength);

		// Find the farthest point(s) along this axis
		float wDir = wLength > 0 ? 1.0f : -1.0f;
		float hDir = hLength > 0 ? 1.0f : -1.0f;

		if (std::abs(wLength) < EPSILON)
		{
			info.hasPoint2 = true;
			sf::Vector2f middle = dimensions[0] * wDir;
			info.point1 = middle + dimensions[1];
			info.point2 = middle - dimensions[1];
		}
		else if (std::abs(hLength) < EPSILON)
		{
			info.hasPoint2 = true;
			sf::Vector2f middle = dimensions[1] * hDir;
			info.point1 = middle + dimensions[0];
			info.point2 = middle - dimensions[0];
		}
		else
		{
			info.point1 = wDir * dimensions[0] + hDir * dimensions[1];
		}

		return info;
	}

}