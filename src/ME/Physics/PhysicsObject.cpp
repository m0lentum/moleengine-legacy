#include <Physics/PhysicsObject.hpp>
#include <iostream>
#include <Space.hpp>

namespace me
{
	void PhysicsObject::applyForce(const sf::Vector2f &force)
	{
		m_props.velocity += (force / m_props.mass); // TODO: account for offset from center
	}

	void PhysicsObject::destroy()
	{
		if (m_space)
			m_space->removeObject(this);
		else
			std::cerr << "Warning: tried to destroy a physics object that is not in a space" << std::endl;
	}


	PhysicsObject::PhysicsObject() :
		GameObject()
	{
	}

	PhysicsObject::PhysicsObject(const Props &props) :
		GameObject(),
		m_props(props)
	{
	}

	PhysicsObject::PhysicsObject(std::shared_ptr<Graphic> graphic) :
		GameObject(graphic)
	{
	}

	PhysicsObject::PhysicsObject(const Props &props, std::shared_ptr<Graphic> graphic) :
		GameObject(graphic),
		m_props(props)
	{
	}

	PhysicsObject::PhysicsObject(const PhysicsObject &copy) :
		GameObject(copy),
		m_props(copy.m_props)
	{
	}

	PhysicsObject::~PhysicsObject()
	{
	}
}