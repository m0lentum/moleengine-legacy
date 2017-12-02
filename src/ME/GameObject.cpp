#include <GameObject.hpp>
#include <IBehavior.hpp>
#include <Space.hpp>
#include <iostream>

namespace me
{
	unsigned int GameObject::numExisting = 0;

	void GameObject::registerSpace(Space *space)
	{
		m_space = space;
	}

	void GameObject::destroy()
	{
		if (m_space)
			m_space->removeObject(m_id);
		else
			std::cerr << "Warning: tried to destroy an object that is not in a space" << std::endl;
	}


	
	void GameObject::addBehavior(IBehavior *behavior)
	{
		m_behaviors.push_back(std::unique_ptr<IBehavior>(behavior));
		behavior->registerParent(this);
	}



	void GameObject::continuousUpdate(sf::Time timeElapsed)
	{
		for (auto &beh : m_behaviors)
		{
			beh->continuousUpdate(timeElapsed);
		}
	}

	void GameObject::fixedUpdate()
	{
		for (auto &beh : m_behaviors)
		{
			beh->fixedUpdate();
		}
	}

	void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (auto &beh : m_behaviors)
		{
			beh->draw(target, states);
		}
	}


	GameObject::GameObject() :
		m_space(NULL),
		m_id(numExisting++)
	{
	}

	GameObject::GameObject(std::initializer_list<IBehavior*> behaviors) :
		m_space(NULL),
		m_id(numExisting++)
	{
		for (auto i = behaviors.begin(); i != behaviors.end(); i++)
		{
			addBehavior(*i);
		}
	}

	GameObject::GameObject(const GameObject &copy) :
		m_space(NULL),
		m_id(numExisting++)
	{
		// Deep copy m_behaviors
		for (auto &beh : copy.m_behaviors)
		{
			addBehavior(beh->clone());
		}
	}

	GameObject::~GameObject()
	{
	}
}