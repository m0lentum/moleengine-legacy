#include <Physics/Physics.hpp>
#include <Component.hpp>
#include <Space.hpp>
#include <Physics/RigidBody.hpp>
#include <vector>
#include <cmath>
#include <Physics/ICollider.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/ColliderRect.hpp>
#include <Physics/ColliderPolygon.hpp>
#include <Physics/PrimitiveQueries.hpp>
#include <Physics/Collision.hpp>
#include <Graphics/DebugMarkerFactory.hpp>
#include <Physics/VectorMath.hpp>
#include <GameObject.hpp>

namespace me
{
	void Physics::fixedUpdate()
	{
		applyMovement();
		findContacts();
		solveCollisions();
	}

	void Physics::applyMovement()
	{
		m_space->each<RigidBody>([&](Component<RigidBody>& comp)
		{
			RigidBody *rb = comp.getComponent();
			GameObject *parent = comp.getParent();

			if (!rb->isKinematic)
			{
				if (!rb->doesOverrideGravity())
					rb->accelerate(m_gravity * rb->gravityMultiplier);
				else
					rb->accelerate(rb->getGravityOverride());

				rb->velocity -= rb->drag * rb->velocity;
				rb->angularVelocity -= rb->angularDrag * rb->angularVelocity;
			}
				
			parent->move(rb->velocity);
			if (rb->angularVelocity != 0) parent->rotate(VectorMath::radToDeg(rb->angularVelocity));
		});
	}

	void Physics::findContacts()
	{
		std::vector<std::pair<GameObject*, ICollider*> > colliders;
		std::unordered_map<long int, Contact> misses;
		m_collisions.clear();


		// gather all different types of colliders into one vector for iteration
		m_space->each<ColliderCircle>([&](Component<ColliderCircle> &comp)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(comp.getParent(), comp.getComponent()));
		});
		m_space->each<ColliderRect>([&](Component<ColliderRect> &comp)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(comp.getParent(), comp.getComponent()));
		});
		m_space->each<ColliderPolygon>([&](Component<ColliderPolygon> &comp)
		{
			colliders.push_back(std::pair<GameObject*, ICollider*>(comp.getParent(), comp.getComponent()));
		});

		// test collider pairs for contact and push them to the map
		using iter = std::vector<std::pair<GameObject*, ICollider*> >::iterator;
		for (iter i = colliders.begin(); i != colliders.end(); i++)
		{
			for (iter j = i + 1; j != colliders.end(); j++)
			{
				Contact con;
				con.obj1 = i->first;
				con.obj2 = j->first;
				PrimitiveQueries::checkCollision(*(i->second), *(j->second), con);

				if (con.areColliding)
				{
					con.rb1 = i->second->getRigidBody();
					con.rb2 = j->second->getRigidBody();

					m_collisions.push_back(con);
				}
				else
				{
					misses.emplace(contactID(con.obj1->getID(), con.obj2->getID()), con);
				}
			}
		}

		m_missedPairs = misses;
	}

	void Physics::solveCollisions()
	{
		// solve contact forces
		for (int i = 0; i < m_solverIterations; i++)
		{
			for (auto &coll : m_collisions)
			{
				if (coll.rb1 && coll.rb2)
				{
					sf::Vector2f point = coll.manifold[0];
					// apply force at the average of the two contact points. naive, not final
					if (coll.manifold[1].x != 0 || coll.manifold[1].y != 0) point = (coll.manifold[0] + coll.manifold[1]) * 0.5f;

					sf::Vector2f offset1 = point - coll.obj1->getPosition();
					sf::Vector2f offset2 = point - coll.obj2->getPosition();

					sf::Vector2f pointVel1 = coll.rb1->getPointVelocity(offset1);
					sf::Vector2f pointVel2 = coll.rb2->getPointVelocity(offset2);
					float collVel = VectorMath::dot(coll.rb2->getPointVelocity(offset2) - coll.rb1->getPointVelocity(offset1), coll.normal);

					if (collVel > 0)
					{
						if (!coll.rb1->isKinematic || !coll.rb2->isKinematic)
						{
							float e = std::min(coll.rb1->elasticity, coll.rb2->elasticity);

							float imp = (1.0f + e) * collVel;
							if (coll.rb1->isKinematic)
							{
								imp /= coll.rb2->mass.inverted + coll.rb2->momentOfInertia.inverted * std::pow(VectorMath::cross2D(offset2, coll.normal), 2);
								coll.rb2->applyImpulse(-imp * coll.normal, offset2);
							}
							else if (coll.rb2->isKinematic)
							{
								imp /= coll.rb1->mass.inverted + coll.rb1->momentOfInertia.inverted * std::pow(VectorMath::cross2D(offset1, coll.normal), 2);
								coll.rb1->applyImpulse(imp * coll.normal, offset1);
							}
							else
							{
								imp /= coll.rb1->mass.inverted + coll.rb2->mass.inverted
									+ coll.rb1->momentOfInertia.inverted * std::pow(VectorMath::cross2D(offset1, coll.normal), 2)
									+ coll.rb2->momentOfInertia.inverted * std::pow(VectorMath::cross2D(offset2, coll.normal), 2);
								coll.rb1->applyImpulse(imp * coll.normal, offset1);
								coll.rb2->applyImpulse(-imp * coll.normal, offset2);
							}
						}
					}
				}
			}
		}

		// resolve penetration if necessary, using linear projection
		for (auto &coll : m_collisions)
		{
			if (coll.rb1 && coll.rb2 && VectorMath::lengthSquared(coll.penetration) > 0.01f)
			{
				if (coll.rb1->isKinematic)
				{
					if (!coll.rb2->isKinematic) coll.obj2->move(coll.penetration * -penResolveFactor);
				}
				else if (coll.rb2->isKinematic)
				{
					coll.obj1->move(coll.penetration * penResolveFactor);
				}
				else
				{
					coll.obj1->move(coll.penetration * penResolveFactor * 0.5f);
					coll.obj2->move(coll.penetration * penResolveFactor * -0.5f);
				}
			}
		}
	}

	long int Physics::contactID(int id1, int id2)
	{
		if (id1 > id2) return id1 + (id2 << 16);
		else return id2 + (id1 << 16);
	}


	Physics::Physics() :
		m_solverIterations(defaultSolverIterations)
	{
	}

	Physics::Physics(const sf::Vector2f &gravity) :
		m_gravity(gravity),
		m_solverIterations(defaultSolverIterations)
	{
	}

	Physics::~Physics()
	{
	}
}