#include <GameObject.hpp>

namespace me
{
	unsigned int GameObject::numExisting = 0;


	void GameObject::destroy()
	{
		// in progress
	}

	GameObject::GameObject(Space *space) :
		m_space(space),
		m_id(numExisting++)
	{
	}

	GameObject::~GameObject()
	{
	}
}