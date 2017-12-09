#include <GameObject.hpp>

namespace me
{
	unsigned int GameObject::numExisting = 0;


	void GameObject::destroy()
	{
		m_isAlive = false;
		for (auto &unit : m_components)
		{
			unit.second->m_isAlive = false;
		}
	}

	GameObject::GameObject(Space *space) :
		m_space(space),
		m_id(numExisting++),
		m_isAlive(true)
	{
	}

	GameObject::GameObject(GameObject&& move) :
		sf::Transformable(std::move(move)),
		m_space(std::move(move.m_space)),
		m_id(std::move(move.m_id)),
		m_components(std::move(move.m_components)),
		m_isAlive(std::move(move.m_isAlive))
	{
		for (auto &unit : m_components)
		{
			unit.second->m_parent = this;
		}
	}

	GameObject& GameObject::operator=(GameObject&& other)
	{
		if (this != &other)
		{
			sf::Transformable::operator=(other);
			m_space = std::move(other.m_space);
			m_id = std::move(other.m_id);
			m_components = std::move(other.m_components);
			m_isAlive = std::move(other.m_isAlive);

			for (auto &unit : m_components)
			{
				unit.second->m_parent = this;
			}
		}

		return *this;
	}

	GameObject::~GameObject()
	{
	}
}