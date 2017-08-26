#include <GameObject.hpp>
#include <Space.hpp>
#include <iostream>

namespace me
{
	void GameObject::continuousUpdate(const sf::Time &timeElapsed)
	{
		if (m_graphic) m_graphic->continuousUpdate(timeElapsed);
	}

	void GameObject::fixedUpdate()
	{
		//A generic GameObject does nothing on fixed update
	}

	void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		if (m_graphic)
		{
			states.transform *= getTransform();
			m_graphic->draw(target, states);
		}
	}

	void GameObject::registerSpace(Space *space)
	{
		m_space = space;
	}

	void GameObject::destroy()
	{
		if (m_space)
			m_space->removeObject(this);
		else
			std::cerr << "Cannot destroy an object that is not in a space" << std::endl;
	}

	void GameObject::setGraphic(std::shared_ptr<Graphic> graphic)
	{
		m_graphic = graphic;
	}



	GameObject::GameObject() :
		m_space(NULL)
	{
	}

	GameObject::GameObject(std::shared_ptr<Graphic> graphic) :
		m_space(NULL),
		m_graphic(graphic)
	{
	}

	GameObject::GameObject(const GameObject &copy) :
		m_space(NULL),
		m_graphic(new Graphic(*copy.m_graphic))
	{
	}

	GameObject::~GameObject()
	{
	}
}