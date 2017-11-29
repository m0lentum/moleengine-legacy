#include <Graphics/GraphicController.hpp>
#include <Graphics/Graphic.hpp>
//#include <iostream>

namespace me
{
    std::string GraphicController::getType()
    {
        return "graphic";
    }

    void GraphicController::draw(sf::RenderTarget& target, sf::RenderStates states)
    {
        for (auto &comp : m_components)
        {
            static_cast<Graphic*>(comp.second)->draw(target, states);
        }
    }


    GraphicController::GraphicController()
    {
    }

    GraphicController::~GraphicController()
    {
    }
}