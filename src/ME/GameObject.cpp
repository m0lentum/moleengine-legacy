#include <GameObject.hpp>
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


	void GameObject::addComponent(IComponent *component)
	{
		m_components.emplace(component->getType(), std::unique_ptr<IComponent>(component));
		component->registerParent(this);

		if (m_space) m_space->addComponent(component);
	}

	void GameObject::removeComponent(const std::string &type)
	{
		m_components.erase(type);
	}

	IComponent * GameObject::getComponent(const std::string &type) const
	{
		return m_components.at(type).get();
	}

	const std::unordered_map<std::string, std::unique_ptr<IComponent> > & GameObject::getAllComponents() const
	{
		return m_components;
	}



	GameObject::GameObject() :
		m_space(NULL),
		m_id(numExisting++)
	{
	}

	GameObject::GameObject(std::initializer_list<IComponent*> components) :
		m_space(NULL),
		m_id(numExisting++)
	{
		for (auto i = components.begin(); i != components.end(); i++)
		{
			addComponent(*i);
		}
	}

	GameObject::GameObject(const GameObject &copy) :
		m_space(NULL),
		m_id(numExisting++)
	{
		// Deep copy m_components
		for (auto &comp : copy.m_components)
		{
			m_components.emplace(comp.first, comp.second->clone());
		}
	}

	GameObject::~GameObject()
	{
	}
}