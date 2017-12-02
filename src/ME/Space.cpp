#include <Space.hpp>
#include <Physics/CollisionChecker.hpp>
//#include <iostream>

namespace me
{
	void Space::continuousUpdate(sf::Time timeElapsed)
	{
		for (auto &cont : m_controllers)
		{
			cont.second->continuousUpdate(timeElapsed);
		}

		for (auto &obj : m_objects)
		{
			obj.second->continuousUpdate(timeElapsed);
		}
	}
	

	void Space::fixedUpdate()
	{
		// Remove all objects marked for destruction
		while (!m_toDestroy.empty())
		{
			unsigned int id = m_toDestroy.front();
			// Remove reference to object from all controllers controlling it
			/*for (auto &item : m_objects[id]->getAllComponents())
			{
				if (m_controllers.count(item.first) > 0)
				{
					m_controllers[item.first]->removeComponent(id);
				}
			}*/

			m_objects.erase(id);
			m_toDestroy.pop();
		}

		for (auto &cont : m_controllers)
		{
			cont.second->fixedUpdate();
		}

		for (auto &obj : m_objects)
		{
			obj.second->fixedUpdate();
		}
	}
	

	void Space::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (auto &cont : m_controllers)
		{
			cont.second->draw(target, states);
		}

		for (auto &obj : m_objects)
		{
			obj.second->draw(target, states);
		}
	}

	/* TODO: move this stuff into a Controller
	void Space::handleCollisions()
	{
		// TODO: add optimization through spatial partitioning

		for (auto i = m_physicsObjects.begin(); i != m_physicsObjects.end(); i++)
		{
			auto j = i;
			j++;
			for (; j != m_physicsObjects.end(); j++)
			{
				CollisionInfo info;
				info.obj1 = *i;
				info.obj2 = *j;
				CollisionChecker::checkCollision((*i)->getCollider(), (*j)->getCollider(), info);

				if (info.areColliding)
				{
					// TODO: onCollision events for objects
					resolveCollision(info);
				}
			}
		}
	}


	void Space::resolveCollision(const CollisionInfo &info)
	{
		//info.obj1->move(info.penetration);
		// TODO: physics
	}
	*/



	void Space::addObject(GameObject * object)
	{
		m_objects.emplace(object->getID(), std::unique_ptr<GameObject>(object));
		object->registerSpace(this);

		/*for (auto &comp : object->getAllComponents())
		{
			if (m_controllers.count(comp.first) > 0)
			{
				m_controllers[comp.first]->registerComponent(comp.second.get());
			}
		}*/
	}

	void Space::removeObject(const unsigned int id)
	{
		m_toDestroy.push(id);
	}
	
	void Space::addController(IController *controller)
	{
		m_controllers.emplace(controller->getType(), std::unique_ptr<IController>(controller));
	}

	void Space::removeController(const std::string &id)
	{
		m_controllers.erase(id);
	}

	void Space::addComponent(IComponent *component)
	{
		std::string type = component->getType();
		if (m_controllers.count(type) > 0)
		{
			m_controllers[type]->registerComponent(component);
		}
	}

	Space::Space()
	{
	}

	Space::Space(const Space &copy)
	{
		// TODO: deep copy the whole dang thing
	}

	Space::~Space()
	{
	}
}