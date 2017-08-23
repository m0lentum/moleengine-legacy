#include <GameObject.hpp>

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

	void GameObject::setGraphic(std::shared_ptr<Graphic> graphic)
	{
		m_graphic = graphic;
	}

	GameObject::GameObject()
	{
	}

	GameObject::GameObject(std::shared_ptr<Graphic> graphic) :
		m_graphic(graphic)
	{
	}

	GameObject::GameObject(const GameObject &copy) :
		m_graphic(new Graphic(*copy.m_graphic))
	{
	}

	GameObject::~GameObject()
	{
	}
}