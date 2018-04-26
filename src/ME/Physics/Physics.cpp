#include <Physics/Physics.hpp>
#include <Component.hpp>
#include <Space.hpp>
#include <Physics/RigidBody.hpp>
#include <vector>
#include <Physics/ICollider.hpp>
#include <Physics/ColliderCircle.hpp>
#include <Physics/ColliderRect.hpp>
#include <Physics/ColliderPolygon.hpp>
#include <Physics/PrimitiveQueries.hpp>
#include <Physics/Contact.hpp>
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
		for (int i = 0; i < m_solverIterations; i++)
		{
			for (auto &coll : m_collisions)
			{
				if (coll.rb1 && coll.rb2)
				{
					float collVel = VectorMath::dot(coll.rb2->velocity - coll.rb1->velocity, coll.normal);

					if (collVel > 0)
					{
						if (!coll.rb1->isKinematic || !coll.rb2->isKinematic)
						{
							float e = std::min(coll.rb1->elasticity, coll.rb2->elasticity);

							float imp = (1.0f + e) * collVel;
							if (coll.rb1->isKinematic)
							{
								imp *= coll.rb2->mass;
								coll.rb2->applyForce(-imp * coll.normal);
							}
							else if (coll.rb2->isKinematic)
							{
								imp *= coll.rb1->mass;
								coll.rb1->applyForce(imp * coll.normal);
							}
							else
							{
								imp /= 1 / coll.rb1->mass + 1 / coll.rb2->mass;
								coll.rb1->applyForce(imp * coll.normal);
								coll.rb2->applyForce(-imp * coll.normal);
							}
						}
					}
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