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

		sf::Vector2f normals[4];
		normals[0] = VectorMath::rotateDeg(sf::Vector2f(1, 0), info.obj1->getRotation());
		normals[1] = VectorMath::leftNormal(normals[0]);
		normals[2] = VectorMath::rotateDeg(sf::Vector2f(1, 0), info.obj2->getRotation());
		normals[3] = VectorMath::leftNormal(normals[2]);

		float hw[4] = { rect1.getHalfWidth() * info.obj1->getScale().x,
			rect1.getHalfHeight() * info.obj1->getScale().y, 
			rect2.getHalfWidth() * info.obj2->getScale().x,
			rect2.getHalfHeight() * info.obj2->getScale().y };


		if (std::abs(VectorMath::dot(normals[0], normals[3])) < EPSILON)
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
		else if (std::abs(VectorMath::dot(normals[0], normals[2])) < EPSILON)
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
			// Boxes differ in orientation (one contact point). Go through each of the four potential separating normals
			
			sf::Vector2f dimensions[4];
			for (int i = 0; i < 4; i++) dimensions[i] = normals[i] * hw[i];

			// flip all the normals to point away from obj1 and towards obj2
			for (int i = 0; i < 4; i++)
			{
				if (VectorMath::dot(normals[i], distance) < 0)
				{
					normals[i] = -normals[i];
				}
			}

			int penAxis = 0;
			float penDepth = 100000;
			for (int i = 0; i < 4; i++)
			{
				int startIndex = i < 2 ? 2 : 0;

				float pen = rectWidthOnAxis(dimensions[startIndex], dimensions[startIndex + 1], normals[i]) + hw[i] - std::abs(VectorMath::dot(distance, normals[i]));
				
				if (pen < 0) return;
				else if (pen < penDepth)
				{
					penDepth = pen;
					penAxis = i;
				}
			}

			info.areColliding = true;
			info.penetration = -penDepth * normals[penAxis];

			// point of contact is on the box that doesn't own the axis of shortest separation
			bool pointOnObj2 = penAxis < 2;

			// find the extreme point on the box that owns the contact point in the direction opposite to the penetration
			if (pointOnObj2)
			{
				for (int i = 2; i <= 3; i++) if (VectorMath::dot(dimensions[i], info.penetration) < 0) dimensions[i] = -dimensions[i];

				info.manifold[0] = info.obj2->getPosition() + dimensions[2] + dimensions[3] - info.penetration; // this works because the normals have been flipped towards obj2
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
		transformPolyPoints(edges, info.obj1);
		std::vector<sf::Vector2f> normals = poly.getAxes();
		rotatePolyNormals(normals, info.obj1->getRotation());
		float radius = circle.getRadius();

		// With the circle we also need to check the axis between its center and the closest point to it
		sf::Vector2f closestPoint = info.obj1->getPosition() + polyWidthOnAxis(edges, distance).point1;
		sf::Vector2f toCircle = info.obj2->getPosition() - closestPoint;
		normals.push_back(VectorMath::normalize(toCircle));

		float penDepth = 100000;
		sf::Vector2f penAxis;
		for (auto &axis : normals)
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
		transformPolyPoints(edges, info.obj1);
		std::vector<sf::Vector2f> normals = poly.getAxes();
		rotatePolyNormals(normals, info.obj1->getRotation());

		float angle = VectorMath::degToRad(info.obj2->getRotation());
		sf::Vector2f rectWAxis = sf::Vector2f(std::cos(angle), std::sin(angle));
		sf::Vector2f rectHAxis = VectorMath::leftNormal(rectWAxis);
		normals.push_back(rectWAxis);
		normals.push_back(rectHAxis);

		sf::Vector2f rectDimensions[2] = { rectWAxis * rect.getHalfWidth(), rectHAxis * rect.getHalfHeight() };

		float penDepth = 100000;
		sf::Vector2f penAxis;
		for (auto &axis : normals)
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

		std::vector<sf::Vector2f> points[2] = { transformPolyPoints(poly1.getPoints(), info.obj1), transformPolyPoints(poly2.getPoints(), info.obj2) };
		std::vector<sf::Vector2f> normals[2] = { rotatePolyNormals(poly1.getNormals(), info.obj1->getRotation()), rotatePolyNormals(poly2.getNormals(), info.obj2->getRotation()) };


		float penDepth = 100000;
		int penAxisIndex;
		int penAxisOwner;
		for (int owner = 0, other = 1; owner <= 1; owner++, other--)
		{
			for (std::vector<sf::Vector2f>::size_type i = 0; i < normals[owner].size(); i++)
			{
				float distOnAxis = VectorMath::dot(normals[owner][i], distance);

				if (distOnAxis < 0) continue; // (test, might not work) discard the axis immediately if it faces away from the other object

				float w1 = VectorMath::dot(normals[owner][i], points[owner][i]);
				float w2 = polyHalfwidthOnAxis(points[other], -normals[owner][i]);

				float pen = w1 + w2 - distOnAxis;

				if (pen < 0) return;

				if (pen < penDepth)
				{
					penAxisIndex = i;
					penAxisOwner = owner;
					penDepth = pen;
				}
			}

			distance = -distance;
		}

		// there is a collision, figure out the point(s)
		info.areColliding = true;
		info.penetration = normals[penAxisOwner][penAxisIndex] * (penAxisOwner == 0 ? -penDepth : penDepth);

		int other = penAxisOwner == 0 ? 1 : 0;
		int otherEdgeIndex = findOppositePolyEdge(normals[other], normals[penAxisOwner][penAxisIndex]);

		if (std::abs(VectorMath::dot(VectorMath::leftNormal(normals[penAxisOwner][penAxisIndex]), normals[other][otherEdgeIndex])) < EPSILON)
		{
			// two parallel edges, two contact points
			sf::Vector2f pos[2] = { info.obj1->getPosition(), info.obj2->getPosition() };
			int sizes[2] = { points[0].size(), points[1].size() };
			sf::Vector2f pts[4] =
			{	pos[penAxisOwner] + points[penAxisOwner][penAxisIndex],
				pos[penAxisOwner] + points[penAxisOwner][(penAxisIndex + 1) % sizes[penAxisOwner]],
				pos[other] + points[other][otherEdgeIndex],
				pos[other] + points[other][(otherEdgeIndex + 1) % sizes[other]] };
			
			
			sf::Vector2f edge1 = pts[1] - pts[0];
			sf::Vector2f dir = VectorMath::normalize(edge1);
			float dot = VectorMath::dot(dir, pts[3] - pts[0]);
			
			if (dot > 0) info.manifold[0] = pts[0] + dot * dir;
			else info.manifold[0] = pts[0];

			dot = VectorMath::dot(dir, pts[2] - pts[0]);
			if (dot < VectorMath::dot(dir, edge1)) info.manifold[1] = pts[0] + dot * dir;
			else info.manifold[1] = pts[1];

			for (auto &vec : info.manifold)
			{
				vec += info.penetration / (other == 0 ? -2.0f : 2.0f);
			}
		}
		else
		{
			// single contact point. test both ends of the closest edge
			float p1 = VectorMath::dot(normals[penAxisOwner][penAxisIndex], points[other][otherEdgeIndex]);
			int p2Index = (otherEdgeIndex + 1) % points[other].size();
			float p2 = VectorMath::dot(normals[penAxisOwner][penAxisIndex], points[other][p2Index]);
			
			sf::Vector2f p = p1 < p2 ? points[other][otherEdgeIndex] : points[other][p2Index];
			sf::Vector2f pos = other == 0 ? info.obj1->getPosition() : info.obj2->getPosition();

			info.manifold[0] = pos + p + info.penetration / (other == 0 ? 2.0f : -2.0f);
		}
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

	std::vector<sf::Vector2f> PrimitiveQueries::transformPolyPoints(const std::vector<sf::Vector2f> &vecs, GameObject *obj)
	{
		std::vector<sf::Vector2f> result;
		result.reserve(vecs.size());
		for (auto &vec : vecs)
		{
			result.push_back(obj->getTransform() * vec - obj->getPosition());
		}

		return result;
	}

	std::vector<sf::Vector2f> PrimitiveQueries::rotatePolyNormals(const std::vector<sf::Vector2f> &vecs, float angle)
	{
		std::vector<sf::Vector2f> result;
		result.reserve(vecs.size());
		for (auto &vec : vecs)
		{
			result.push_back(VectorMath::rotateDeg(vec, angle));
		}

		return result;
	}

	float PrimitiveQueries::polyHalfwidthOnAxis(const std::vector<sf::Vector2f> &points, const sf::Vector2f &axis)
	{
		// hill-climb to find the farthest point in the direction
		float curr = VectorMath::dot(axis, points[0]);
		int last = points.size() - 1;
		float next = VectorMath::dot(axis, points[last]);

		int i = last, increment =  -1;

		if (next < curr)
		{
			increment = 1;
			i = 1;
			next = VectorMath::dot(axis, points[i]);
		}

		while (next > curr)
		{
			curr = next;
			i += increment;
			next = VectorMath::dot(axis, points[i]);
		}
		
		return curr;
	}

	int PrimitiveQueries::findOppositePolyEdge(const std::vector<sf::Vector2f> &normals, const sf::Vector2f &axis)
	{
		// hill-climb to find the normal closest to opposite the axis
		float curr = VectorMath::dot(axis, normals[0]);
		int last = normals.size() - 1;
		float next = VectorMath::dot(axis, normals[last]);

		int i = 0, nextI = last, increment = -1;

		if (next > curr)
		{
			increment = 1;
			nextI = 1;
			next = VectorMath::dot(axis, normals[nextI]);
		}

		while (next < curr)
		{
			curr = next;
			i = nextI;
			nextI += increment;
			next = VectorMath::dot(axis, normals[nextI]);
		}

		return i;
	}

}