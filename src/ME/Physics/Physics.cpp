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
#include <Graphics/DebugMarkerFactory.hpp>

namespace me
{
	void Physics::fixedUpdate()
	{
		applyMovement();
		findContacts();
		solveContacts();
	}

	void Physics::applyMovement()
	{
		m_space->each<RigidBody>([&](Component<RigidBody>& comp)
		{
			RigidBody *rb = comp.getComponent();
			GameObject *parent = comp.getParent();

			if (!rb->isStatic)
			{
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
				parent->rotate(rb->angularVelocity);
			}
		});
	}

	void Physics::findContacts()
	{
		std::vector<std::pair<GameObject*, ICollider*> > colliders;
		std::unordered_map<long int, Contact> contacts;

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
					// immediately resolve penetration
					RigidBody *rb1 = i->second->getRigidBody();
					RigidBody *rb2 = j->second->getRigidBody();

					if (rb1 && rb2)
					{
						if (rb1->isStatic || rb1->isKinematic)
						{
							if (!(rb2->isStatic || rb2->isKinematic))
								con.obj2->move(-penResolveFactor * con.penetration);
						}
						else if (rb2->isStatic || rb2->isKinematic)
						{
							con.obj1->move(penResolveFactor * con.penetration);
						}
						else
						{
							con.obj1->move(penResolveFactor * 0.5f * con.penetration);
							con.obj2->move(penResolveFactor * -0.5f * con.penetration);
						}
					}
				}

				contacts.emplace(contactID(con.obj1->getID(), con.obj2->getID()), con);
			}
		}

		m_contacts = contacts;
	}

	void Physics::solveContacts()
	{
		for (int i = 0; i < m_solverIterations; i++)
		{
			for (auto &con : m_contacts)
			{
				if (con.second.areColliding)
				{
					// TODO continue from here
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