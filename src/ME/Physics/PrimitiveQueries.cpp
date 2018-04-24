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

		float penDepth = circle1.getRadius() + circle2.getRadius() - VectorMath::length(distance);
		if (penDepth > 0) // A collision occurred
		{
			info.areColliding = true;
			info.penetration = axis * -penDepth;
			info.manifold[0] = info.obj1->getPosition() + axis * circle1.getRadius() + (info.penetration * 0.5f);
		}
	}

	void PrimitiveQueries::circleRect(const ColliderCircle &circle, const ColliderRect &rect, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		rectCircle(rect, circle, info);
		std::swap(info.obj1, info.obj2);
		info.penetration = -info.penetration;
	}

	void PrimitiveQueries::circlePoly(const ColliderCircle &circle, const ColliderPolygon &poly, Contact &info)
	{
		std::swap(info.obj1, info.obj2);
		polyCircle(poly, circle, info);
		std::swap(info.obj1, info.obj2);
		info.penetration = -info.penetration;
	}


	//===================  RECT and X  ===================
	void PrimitiveQueries::rectCircle(const ColliderRect &rect, const ColliderCircle &circle, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		float rectDimensions[2] = { rect.getHalfWidth(), rect.getHalfHeight() };
		sf::Vector2f rectNormals[2];
		rectNormals[0] = VectorMath::rotateDeg(sf::Vector2f(1, 0), info.obj1->getRotation());
		rectNormals[1] = VectorMath::leftNormal(rectNormals[0]);
		float radius = circle.getRadius();

		float penDepth = 100000;
		int penAxisIndex;
		for (int i = 0; i < 2; i++)
		{
			float distOnAxis = VectorMath::dot(distance, rectNormals[i]);

			if (distOnAxis < 0)
			{
				distOnAxis = -distOnAxis;
				rectNormals[i] = -rectNormals[i];
			}

			float pen = rectDimensions[i] + radius - distOnAxis;

			if (pen < 0) return;

			if (pen < penDepth)
			{
				penDepth = pen;
				penAxisIndex = i;
			}
		}

		// check the axis defined by the closest point to the circle
		sf::Vector2f closest = rectNormals[0] * rectDimensions[0] + rectNormals[1] * rectDimensions[1];
		sf::Vector2f axis = VectorMath::normalize(distance - closest);

		float distOnAxis = std::abs(VectorMath::dot(distance, axis));
		float rectW = std::abs(VectorMath::dot(rectNormals[0], axis)) * rectDimensions[0] + std::abs(VectorMath::dot(rectNormals[1], axis)) * rectDimensions[1];

		float pen = rectW + radius - distOnAxis;

		if (pen < 0) return;


		info.areColliding = true;

		if (pen < penDepth)
		{
			info.penetration = -pen * axis;
			info.manifold[0] = info.obj1->getPosition() + closest + (info.penetration * 0.5f);
		}
		else
		{
			info.penetration = -penDepth * rectNormals[penAxisIndex];
			info.manifold[0] = info.obj2->getPosition() - (radius * rectNormals[penAxisIndex]) - (info.penetration * 0.5f);
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
			info.manifold[0] = is.point1 + (info.penetration * 0.5f);
			info.manifold[1] = is.point2 + (info.penetration * 0.5f);
		}
		else if (std::abs(VectorMath::dot(normals[0], normals[2])) < EPSILON)
		{
			// Boxes are perpendicular, two contact points
			EdgeEdgeIntersection is = intersectParallelRects(info.obj1->getTransform(), info.obj1->getPosition(), 
				info.obj2->getPosition(), hw[0], hw[1], hw[3], hw[2]);

			if (!is.doesIntersect) return;

			info.areColliding = true;
			info.penetration = is.penetration;
			info.manifold[0] = is.point1 + (info.penetration * 0.5f);
			info.manifold[1] = is.point2 + (info.penetration * 0.5f);
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

				float pen = rectHalfwidthOnAxis(dimensions[startIndex], dimensions[startIndex + 1], normals[i]) + hw[i] - std::abs(VectorMath::dot(distance, normals[i]));
				
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

				info.manifold[0] = info.obj2->getPosition() + dimensions[2] + dimensions[3] - (info.penetration * 0.5f); // this works because the normals have been flipped towards obj2
			}
			else
			{
				for (int i = 0; i <= 1; i++) if (VectorMath::dot(dimensions[i], info.penetration) > 0) dimensions[i] = -dimensions[i];

				info.manifold[0] = info.obj1->getPosition() + dimensions[0] + dimensions[1] + (info.penetration * 0.5f);
			}
		}
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
		
		std::vector<sf::Vector2f> polyPoints = poly.getPoints();
		transformPolyPoints(polyPoints, info.obj1);
		std::vector<sf::Vector2f> polyNormals = poly.getNormals();
		rotatePolyNormals(polyNormals, info.obj1->getRotation());
		float radius = circle.getRadius();

		float penDepth = 100000;
		int penAxisIndex;
		for (std::vector<sf::Vector2f>::size_type i = 0; i < polyNormals.size(); i++)
		{
			float distOnAxis = VectorMath::dot(distance, polyNormals[i]);

			// discard the axis immediately if it faces away from the other object
			// (I don't have proof that this always works but it seems to)
			if (distOnAxis < 0) continue;

			float polyW = VectorMath::dot(polyPoints[i], polyNormals[i]);

			float pen = polyW + radius - distOnAxis;

			if (pen < 0) return;

			if (pen < penDepth)
			{
				penDepth = pen;
				penAxisIndex = i;
			}
		}

		// we also need to test the axis defined by the vector between the circle center and the closest poly point
		int closestPt = findClosestPolyPoint(polyPoints, distance);
		sf::Vector2f toCircle = distance - polyPoints[closestPt];
		sf::Vector2f axis = VectorMath::normalize(toCircle);

		float polyW = polyHalfwidthOnAxis(polyPoints, axis);

		float pen = polyW + radius - VectorMath::dot(distance, axis);

		if (pen < 0) return;


		info.areColliding = true;

		if (pen < penDepth)
		{
			// last axis was the one with shortest penetration
			info.penetration = -pen * axis;
			info.manifold[0] = info.obj1->getPosition() + polyPoints[closestPt] + (info.penetration * 0.5f);
		}
		else
		{
			info.penetration = -penDepth * polyNormals[penAxisIndex];
			info.manifold[0] = info.obj2->getPosition() - (radius * polyNormals[penAxisIndex]) - (info.penetration * 0.5f);
		}
	}

	void PrimitiveQueries::polyRect(const ColliderPolygon &poly, const ColliderRect &rect, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		std::vector<sf::Vector2f> polyPoints = poly.getPoints();
		transformPolyPoints(polyPoints, info.obj1);
		std::vector<sf::Vector2f> polyNormals = poly.getNormals();
		rotatePolyNormals(polyNormals, info.obj1->getRotation());

		sf::Vector2f rectAxes[2] = { VectorMath::rotateDeg(sf::Vector2f(1, 0), info.obj2->getRotation()), VectorMath::rotateDeg(sf::Vector2f(0, 1), info.obj2->getRotation()) };
		float rectWidths[2] = { rect.getHalfWidth(), rect.getHalfHeight() };
		sf::Vector2f rectDimensions[2] = { rectAxes[0] * rectWidths[0], rectAxes[1] * rectWidths[1] };

		float penDepth = 100000;
		bool rectOwnsAxis;
		int penAxisIndex;
		for (int i = 0; i < 2; i++) // rect axes
		{
			float distOnAxis = -VectorMath::dot(rectAxes[i], distance);
			if (distOnAxis < 0)
			{
				distOnAxis = -distOnAxis;
				rectAxes[i] = -rectAxes[i];
				rectDimensions[i] = -rectDimensions[i];
			}

			float w1 = rectWidths[i];
			float w2 = polyHalfwidthOnAxis(polyPoints, -rectAxes[i]);

			float pen = w1 + w2 - distOnAxis;

			if (pen < 0) return;

			if (pen < penDepth)
			{
				rectOwnsAxis = true;
				penAxisIndex = i;
				penDepth = pen;
			}
		}

		for (std::vector<sf::Vector2f>::size_type i = 0; i < polyNormals.size(); i++)
		{
			float distOnAxis = VectorMath::dot(polyNormals[i], distance);

			if (distOnAxis < 0) continue;

			float w1 = rectHalfwidthOnAxis(rectDimensions[0], rectDimensions[1], polyNormals[i]);
			float w2 = VectorMath::dot(polyNormals[i], polyPoints[i]);

			float pen = w1 + w2 - distOnAxis;

			if (pen < 0) return;

			if (pen < penDepth)
			{
				rectOwnsAxis = false;
				penAxisIndex = i;
				penDepth = pen;
			}
		}

		// There is a collision

		info.areColliding = true;
		if (rectOwnsAxis) info.penetration = rectAxes[penAxisIndex] * penDepth;
		else info.penetration = polyNormals[penAxisIndex] * -penDepth;

		// find out whether or not the colliding edges are parallel
		sf::Vector2f points[4];
		if (rectOwnsAxis)
		{
			int otherEdge = findOppositePolyEdge(polyNormals, rectAxes[penAxisIndex]);
			if (std::abs(VectorMath::dot(polyNormals[otherEdge], VectorMath::leftNormal(rectAxes[penAxisIndex]))) < EPSILON)
			{
				points[0] = polyPoints[otherEdge] + info.obj1->getPosition();
				points[1] = polyPoints[(otherEdge + 1) % polyPoints.size()] + info.obj1->getPosition();
				points[2] = rectDimensions[penAxisIndex] + rectDimensions[(penAxisIndex + 1) % 2] + info.obj2->getPosition();
				points[3] = points[2] - 2.0f * rectDimensions[(penAxisIndex + 1) % 2];
			}
			else
			{
				// just one contact point
				float d1 = -VectorMath::dot(polyPoints[otherEdge], rectAxes[penAxisIndex]);
				int next = (otherEdge + 1) % polyPoints.size();
				float d2 = -VectorMath::dot(polyPoints[next], rectAxes[penAxisIndex]);

				if (d1 > d2) info.manifold[0] = polyPoints[otherEdge] + info.obj1->getPosition() + (info.penetration * 0.5f);
				else info.manifold[0] = polyPoints[next] + info.obj1->getPosition() + (info.penetration * 0.5f);

				return;
			}
		}
		else
		{
			if (std::abs(VectorMath::dot(polyNormals[penAxisIndex], rectAxes[1])) < EPSILON)
			{
				points[2] = rectDimensions[0] + rectDimensions[1] + info.obj2->getPosition();
				points[3] = points[2] - 2.0f * rectDimensions[1];
			}
			else if (std::abs(VectorMath::dot(polyNormals[penAxisIndex], rectAxes[0])) < EPSILON)
			{
				points[2] = rectDimensions[1] + rectDimensions[0] + info.obj2->getPosition();
				points[3] = points[2] - 2.0f * rectDimensions[0];
			}
			else
			{
				// just one contact point, find the farthest corner on the axis of penetration
				for (auto &dim : rectDimensions)
				{
					if (VectorMath::dot(polyNormals[penAxisIndex], dim) > 0) dim = -dim;
				}

				info.manifold[0] = rectDimensions[0] + rectDimensions[1] + info.obj2->getPosition() - (info.penetration * 0.5f);

				return;
			}

			points[0] = polyPoints[penAxisIndex] + info.obj1->getPosition();
			points[1] = polyPoints[(penAxisIndex + 1) % polyPoints.size()] + info.obj1->getPosition();
		}

		// at this point we have two parallel edges, find the middle two points
		if (VectorMath::dot(points[1] - points[0], points[3] - points[2]) > 0)
		{
			// edges must be in opposite directions for the overlap function to return the correct values
			std::swap(points[2], points[3]);
		}

		findOverlapOfEdges(points, info.manifold[0], info.manifold[1]);

		for (auto &vec : info.manifold)
		{
			vec += info.penetration * 0.5f;
		}
	}

	void PrimitiveQueries::polyPoly(const ColliderPolygon &poly1, const ColliderPolygon &poly2, Contact &info)
	{
		sf::Vector2f distance = info.obj2->getPosition() - info.obj1->getPosition();

		std::vector<sf::Vector2f> points[2] = { poly1.getPoints(), poly2.getPoints() };
		transformPolyPoints(points[0], info.obj1);
		transformPolyPoints(points[1], info.obj2);
		std::vector<sf::Vector2f> normals[2] = { poly1.getNormals(), poly2.getNormals() };
		rotatePolyNormals(normals[0], info.obj1->getRotation());
		rotatePolyNormals(normals[1], info.obj2->getRotation());

		float penDepth = 100000;
		int penAxisIndex;
		int penAxisOwner;
		for (int owner = 0, other = 1; owner <= 1; owner++, other--)
		{
			for (std::vector<sf::Vector2f>::size_type i = 0; i < normals[owner].size(); i++)
			{
				float distOnAxis = VectorMath::dot(normals[owner][i], distance);

				if (distOnAxis < 0) continue;

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
			std::vector<sf::Vector2f>::size_type sizes[2] = { points[0].size(), points[1].size() };
			sf::Vector2f pts[4] =
			{	pos[penAxisOwner] + points[penAxisOwner][penAxisIndex],
				pos[penAxisOwner] + points[penAxisOwner][(penAxisIndex + 1) % sizes[penAxisOwner]],
				pos[other] + points[other][otherEdgeIndex],
				pos[other] + points[other][(otherEdgeIndex + 1) % sizes[other]] };
			
			findOverlapOfEdges(pts, info.manifold[0], info.manifold[1]);

			for (auto &vec : info.manifold)
			{
				vec += info.penetration * (other == 0 ? -0.5f : 0.5f);
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

			info.manifold[0] = pos + p + info.penetration * (other == 0 ? 0.5f : -0.5f);
		}
	}



	// ===================================== CLOSEST POINT ============================================

	sf::Vector2f PrimitiveQueries::closestPtOnCircleToPoint(const sf::Vector2f &circlePos, float circleRadius, const sf::Vector2f &point)
	{
		sf::Vector2f diff = point - circlePos;
		float distSquared = VectorMath::lengthSquared(diff);

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

	void PrimitiveQueries::findOverlapOfEdges(const sf::Vector2f pts[4], sf::Vector2f &out1, sf::Vector2f &out2)
	{
		sf::Vector2f edge1 = pts[1] - pts[0];
		sf::Vector2f dir = VectorMath::normalize(edge1);
		float dot = VectorMath::dot(dir, pts[3] - pts[0]);

		if (dot > 0) out1 = pts[0] + dot * dir;
		else out1 = pts[0];

		dot = VectorMath::dot(dir, pts[2] - pts[0]);
		if (dot < VectorMath::dot(dir, edge1)) out2 = pts[0] + dot * dir;
		else out2 = pts[1];
	}

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

	void PrimitiveQueries::transformPolyPoints(std::vector<sf::Vector2f> &vecs, GameObject *obj)
	{
		for (std::vector<sf::Vector2f>::size_type i = 0; i < vecs.size(); i++)
		{
			vecs[i] = obj->getTransform() * vecs[i] - obj->getPosition();
		}
	}

	void PrimitiveQueries::rotatePolyNormals(std::vector<sf::Vector2f> &vecs, float angle)
	{
		for (std::vector<sf::Vector2f>::size_type i = 0; i < vecs.size(); i++)
		{
			vecs[i] = VectorMath::rotateDeg(vecs[i], angle);
		}
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

	float PrimitiveQueries::rectHalfwidthOnAxis(const sf::Vector2f &hw, const sf::Vector2f &hh, const sf::Vector2f &axis)
	{
		return std::abs(VectorMath::dot(hw, axis)) + std::abs(VectorMath::dot(hh, axis));
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

	int PrimitiveQueries::findClosestPolyPoint(const std::vector<sf::Vector2f> &points, const sf::Vector2f &target)
	{
		float curr = VectorMath::lengthSquared(target - points[0]);
		int last = points.size() - 1;
		float next = VectorMath::lengthSquared(target - points[last]);

		int i = 0, nextI = last, increment = -1;

		if (next > curr)
		{
			increment = 1;
			nextI = 1;
			next = VectorMath::lengthSquared(target - points[1]);
		}

		while (next < curr)
		{
			curr = next;
			i = nextI;
			nextI += increment;
			next = VectorMath::lengthSquared(target - points[nextI]);
		}

		return i;
	}
}