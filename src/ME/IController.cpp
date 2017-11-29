#include <IController.hpp>
#include <IComponent.hpp>
//#include <iostream>

namespace me
{
    void IController::registerComponent(IComponent *component)
    {
        m_components.emplace(component->getID(), component);
    }

    void IController::removeComponent(unsigned int id)
    {
        m_components.erase(id);
    }

    void IController::registerSpace(Space *space)
    {
        m_parentSpace = space;
    }
}