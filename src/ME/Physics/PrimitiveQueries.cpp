#include <Physics/PrimitiveQueries.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/ColliderRect.hpp>
#include <Physics/ColliderPolygon.hpp>
#include <Physics/ICollider.hpp>
#include <Physics/Contact.hpp>
#include <Physics/VectorMath.hpp>
#include <GameObject.hpp>
#include <cmath>
#include <iostream>

namespace me
{
	float PrimitiveQueries::EPSILON = 0.03f;


	void PrimitiveQueries::checkCollision(const ICollider &coll1, const ICollider &coll2, Contact &info)
	{
		coll1.findTypeAndCheckCollision(coll2, info);
	}

	//===================  CIRCLE and X  ===================
	void PrimitiveQueries::circleCircle(const ColliderCircle &circle1, const ColliderCircle &circle2, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();
		sf::Vector2f axis = VectorMath::normalize(distance);

		float penDepth = circle1.getRadius() + circle2.getRadius() - VectorMath::getLength(distance);
		if (penDepth > 0) // A collision occurred
		{
			info.areColliding = true;
			info.penetration = axis * -penDepth;
			info.manifold[0] = info.obj1->getPosition() + axis * circle1.getRadius();
		}
	}

	void PrimitiveQueries::circleRect(const ColliderCircle &circle, const ColliderRect &rect, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		rectCircle(rect, circle, info);
	}

	void PrimitiveQueries::circlePoly(const ColliderCircle &circle, const ColliderPolygon &poly, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		polyCircle(poly, circle, info);
	}


	//===================  RECT and X  ===================
	void PrimitiveQueries::rectCircle(const ColliderRect &rect, const ColliderCircle &circle, Contact &info)
	{
		sf::Vector2f closest = closestPtOnRectToPoint(info.obj1->getTransform(), rect.getHalfWidth(), rect.getHalfHeight(), info.obj2->getPosition());
		sf::Vector2f distance = closest - info.obj2->getPosition();
		float radius = circle.getRadius();

		if (VectorMath::getLengthSquared(distance) > 0.0001f)
		{
			float pen = radius - VectorMath::getLength(distance);
			if (pen > 0.0f)
			{
				info.areColliding = true;
				info.penetration = pen * VectorMath::normalize(distance);
				info.manifold[0] = closest;
			}
		}
		else
		{
			info.areColliding = true;

			// Circle center is inside the rect (deep penetration). The axis of minimum penetration is one of the rect's normals.
			sf::Vector2f axis1 = VectorMath::rotateDeg(sf::Vector2f(1, 0), info.obj1->getRotation());
			sf::Vector2f axis2 = VectorMath::leftNormal(axis1);

			sf::Vector2f diff = closest - info.obj1->getPosition();

			float dot1 = VectorMath::dot(axis1, diff);
			float dot2 = VectorMath::dot(axis2, diff);

			float pen1 = rect.getHalfWidth() - std::abs(dot1);
			float pen2 = rect.getHalfHeight() - std::abs(dot2);

			if (pen1 < pen2)
			{
				float sign = dot1 > 0 ? 1.0f : -1.0f;
				info.penetration = -sign * (pen1 + radius) * axis1;
				info.manifold[0] = closest + sign * pen1 * axis1;
			}
			else
			{
				float sign = dot2 > 0 ? 1.0f : -1.0f;
				info.penetration = -sign * (pen2 + radius) * axis2;
				info.manifold[0] = closest + sign * pen2 * axis2;
			}
		}
	}

	void PrimitiveQueries::rectRect(const ColliderRect &rect1, const ColliderRect &rect2, Contact &info)
	{
		sf::Vector2f distance(info.obj2->getPosition() - info.obj1->getPosition());

		sf::Vector2f axes[4];
		axes[0] = VectorMath::rotateDeg(sf::Vector2f(1, 0), info.obj1->getRotation());
		axes[1] = VectorMath::leftNormal(axes[0]);
		axes[2] = VectorMath::rotateDeg(sf::Vector2f(1, 0), info.obj2->getRotation());
		axes[3] = VectorMath::leftNormal(axes[2]);

		float hw[4] = { rect1.getHalfWidth() * info.obj1->getScale().x,
			rect1.getHalfHeight() * info.obj1->getScale().y, 
			rect2.getHalfWidth() * info.obj2->getScale().x,
			rect2.getHalfHeight() * info.obj2->getScale().y };


		if (std::abs(VectorMath::dot(axes[0], axes[3])) < EPSILON)
		{
			// Boxes are parallel, two contact points
			EdgeEdgeIntersection is = intersectParallelRects(info.obj1->getTransform(), info.obj1->getPosition(), 
				info.obj2->getPosition(), hw[0], hw[1], hw[2], hw[3]);
			
			if (!is.doesIntersect) return;

			info.areColliding = true;
			info.penetration = is.penetration;
			info.manifold[0] = is.point1;
			info.manifold[1] = is.point2;
		}
		else if (std::abs(VectorMath::dot(axes[0], axes[2])) < EPSILON)
		{
			// Boxes are perpendicular, two contact points
			EdgeEdgeIntersection is = intersectParallelRects(info.obj1->getTransform(), info.obj1->getPosition(), 
				info.obj2->getPosition(), hw[0], hw[1], hw[3], hw[2]);

			if (!is.doesIntersect) return;

			info.areColliding = true;
			info.penetration = is.penetration;
			info.manifold[0] = is.point1;
			info.manifold[1] = is.point2;
		}
		else
		{
			// Boxes differ in orientation (one contact point). Go through each of the four potential separating axes
			
			sf::Vector2f dimensions[4];
			for (int i = 0; i < 4; i++) dimensions[i] = axes[i] * hw[i];

			// flip all the axes to point away from obj1 and towards obj2
			for (int i = 0; i < 4; i++)
			{
				if (VectorMath::dot(axes[i], distance) < 0)
				{
					axes[i] = -axes[i];
				}
			}

			int penAxis = 0;
			float penDepth = 100000;
			for (int i = 0; i < 4; i++)
			{
				int startIndex = i < 2 ? 2 : 0;

				float pen = rectWidthOnAxis(dimensions[startIndex], dimensions[startIndex + 1], axes[i]) + hw[i] - std::abs(VectorMath::dot(distance, axes[i]));
				
				if (pen < 0) return;
				else if (pen < penDepth)
				{
					penDepth = pen;
					penAxis = i;
				}
			}

			info.areColliding = true;
			info.penetration = -penDepth * axes[penAxis];

			// point of contact is on the box that doesn't own the axis of shortest separation
			bool pointOnObj2 = penAxis < 2;

			// find the extreme point on the box that owns the contact point in the direction opposite to the penetration
			if (pointOnObj2)
			{
				for (int i = 2; i <= 3; i++) if (VectorMath::dot(dimensions[i], info.penetration) < 0) dimensions[i] = -dimensions[i];

				info.manifold[0] = info.obj2->getPosition() + dimensions[2] + dimensions[3] - info.penetration; // this works because the axes have been flipped towards obj2
			}
			else
			{
				for (int i = 0; i <= 1; i++) if (VectorMath::dot(dimensions[i], info.penetration) > 0) dimensions[i] = -dimensions[i];

				info.manifold[0] = info.obj1->getPosition() + dimensions[0] + dimensions[1];
			}
		}
	}

	EdgeEdgeIntersection PrimitiveQueries::intersectParallelRects(const sf::Transform &transform, const sf::Vector2f &pos1, const sf::Vector2f &pos2, float hw1, float hw2, float hw3, float hw4)
	{
		EdgeEdgeIntersection is = intersectAABBs(sf::Vector2f(0, 0), hw1, hw2, transform.getInverse() * pos2, hw3, hw4);

		if (!is.doesIntersect) return is;

		is.penetration = transform * is.penetration - pos1;
		is.point1 = transform * is.point1;
		is.point2 = transform * is.point2;

		return is;
	}

	float PrimitiveQueries::rectWidthOnAxis(const sf::Vector2f &hw, const sf::Vector2f &hh, const sf::Vector2f &axis)
	{
		return std::abs(VectorMath::dot(hw, axis)) + std::abs(VectorMath::dot(hh, axis));
	}

	void PrimitiveQueries::rectPoly(const ColliderRect &rect, const ColliderPolygon &poly, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		polyRect(poly, rect, info);
		std::swap(info.obj1, info.obj2);
		info.penetration = -info.penetration;
	}


	//===================  POLY and X  ===================
	void PrimitiveQueries::polyCircle(const ColliderPolygon &poly, const ColliderCircle &circle, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();
		
		/*std::vector<sf::Vector2f> edges = poly.getEdges();
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
		info.penetration = -penAxis * penDepth;*/
	}

	void PrimitiveQueries::polyRect(const ColliderPolygon &poly, const ColliderRect &rect, Contact &info)
	{
		//BROKEN
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		/*std::vector<sf::Vector2f> edges = poly.getEdges();
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
			PolyAxisInfo w2;// = rectWidthOnAxis(rectDimensions, axis);

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
		info.penetration = -penDepth * penAxis;*/

	}

	void PrimitiveQueries::polyPoly(const ColliderPolygon &poly1, const ColliderPolygon &poly2, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		/*std::vector<sf::Vector2f> edges1 = poly1.getEdges();
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
		info.penetration = -penDepth * penAxis;*/
	}



	// ===================================== CLOSEST POINT ============================================

	sf::Vector2f PrimitiveQueries::closestPtOnCircleToPoint(const sf::Vector2f &circlePos, float circleRadius, const sf::Vector2f &point)
	{
		sf::Vector2f diff = point - circlePos;
		float distSquared = VectorMath::getLengthSquared(diff);

		if (distSquared < circleRadius * circleRadius)
		{
			// Point is in circle
			return point;
		}
		else
		{
			return circlePos + diff * (circleRadius / std::sqrt(distSquared));
		}
	}

	sf::Vector2f PrimitiveQueries::closestPtOnRectToPoint(const sf::Transform &rectTransform, float rectHalfwidth, float rectHalfheight, const sf::Vector2f &point)
	{
		// transform to rect local space
		sf::Vector2f closest = rectTransform.getInverse() * point;

		// in the rectangle's local coordinate space, clamp the circle's position to the (now axis-aligned) box
		if (closest.x > rectHalfwidth) closest.x = rectHalfwidth;
		else if (closest.x < -rectHalfwidth) closest.x = -rectHalfwidth;
		if (closest.y > rectHalfheight) closest.y = rectHalfheight;
		else if (closest.y < -rectHalfheight) closest.y = -rectHalfheight;

		// transform back to global space
		return rectTransform * closest;
	}


	// ======================================= OTHER ===============================================

	EdgeEdgeIntersection PrimitiveQueries::intersectAABBs(const sf::Vector2f &pos1, float hw1, float hh1, const sf::Vector2f &pos2, float hw2, float hh2)
	{
		EdgeEdgeIntersection intersection;

		float distX = pos2.x - pos1.x;
		float penX = (hw1 + hw2) - std::abs(distX);
		if (penX < 0) return intersection;

		float distY = pos2.y - pos1.y;
		float penY = (hh1 + hh2) - std::abs(distY);
		if (penY < 0) return intersection;

		// there is an intersection, figure out which axis has the least penetration
		intersection.doesIntersect = true;
		if (penX > penY)
		{
			float x[4] = { pos1.x - hw1, pos2.x - hw2, pos1.x + hw1, pos2.x + hw2 };
			float y = pos1.y + (distY > 0 ? hh1 : -hh1);
			intersection.point1.x = x[0] > x[1] ? x[0] : x[1];
			intersection.point2.x = x[2] < x[3] ? x[2] : x[3];
			intersection.point1.y = y;
			intersection.point2.y = y;
			
			intersection.penetration.y = distY > 0 ? -penY : penY;
			intersection.penetration.x = 0;
		}
		else
		{
			float y[4] = { pos1.y - hh1, pos2.y - hh2, pos1.y + hh1, pos2.y + hh2 };
			float x = pos1.x + (distX > 0 ? hw1 : -hw1);
			intersection.point1.y = y[0] > y[1] ? y[0] : y[1];
			intersection.point2.y = y[2] < y[3] ? y[2] : y[3];
			intersection.point1.x = x;
			intersection.point2.x = x;

			intersection.penetration.x = distX > 0 ? -penX : penX;
			intersection.penetration.y = 0;
		}

		return intersection;
	}

	void PrimitiveQueries::transformVectors(std::vector<sf::Vector2f> &vecs, GameObject *obj)
	{
		for (auto &vec : vecs)
		{
			vec = obj->getTransform() * vec - obj->getPosition();
		}
	}

	void PrimitiveQueries::rotateVectors(std::vector<sf::Vector2f> &vecs, float angle)
	{
		for (auto &vec : vecs)
		{
			vec = VectorMath::rotateDeg(vec, angle);
		}
	}

	PrimitiveQueries::PolyAxisInfo PrimitiveQueries::polyWidthOnAxis(const std::vector<sf::Vector2f> &edges, const sf::Vector2f &axis)
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

}