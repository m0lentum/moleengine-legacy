#include <Devtools/SpawnerWindow.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <GameObject.hpp>
#include <Input/MouseController.hpp>
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

namespace me
{
    void SpawnerWindow::continuousUpdate(sf::Time timeElapsed)
    {
        ImGui::Begin("Spawner");

        for (auto &item : m_items)
        {
            if (ImGui::Button(item.name.c_str()))
            {
                m_currentItem = &item;
                m_isPlacingItem = true;
            }
        }

        ImGui::End();
    }

    void SpawnerWindow::addObject(std::string name, std::function<GameObject*(Space*)> createFunction, Graphic previewGraphic)
    {
        m_items.emplace_back(name, createFunction, previewGraphic);
    }

    void SpawnerWindow::addObject(std::string name, std::function<GameObject*(Space*)> createFunction)
    {
        m_items.emplace_back(name, createFunction, m_defaultGraphic);
    }



    void SpawnerWindow::onMousePressed(const sf::Event::MouseButtonEvent& evt)
    {
        if (m_isPlacingItem)
        {
            switch (evt.button)
            {
                case sf::Mouse::Left:
                {
                    GameObject *obj = m_currentItem->createFunction(m_space);
                    obj->setPosition(evt.x, evt.y);
                    m_isPlacingItem = false;
                    break;
                }
                case sf::Mouse::Right:
                    m_isPlacingItem = false;
                    break;
                default:
                    break;
            }
        }
    }


    void SpawnerWindow::draw(sf::Window &target, sf::RenderStates states) const
    {
        if (m_isPlacingItem)
        {
            states.transform.translate(static_cast<sf::Vector2f>(sf::Mouse::getPosition(target)));
            m_currentItem->previewGraphic.draw(static_cast<sf::RenderWindow&>(target), states);
        }
    }


    SpawnerWindow::SpawnerWindow(Space *space) :
        m_space(space),
        m_isPlacingItem(false)
    {
    }
    
    SpawnerWindow::SpawnerWindow(SpawnerWindow &&move) :
        m_space(std::move(move.m_space)),
        m_items(std::move(move.m_items)),
        m_currentItem(std::move(move.m_currentItem)),
        m_defaultGraphic(std::move(move.m_defaultGraphic)),
        m_isPlacingItem(std::move(move.m_isPlacingItem))
    {
    }
    
    SpawnerWindow::~SpawnerWindow() {}

    SpawnerWindow& SpawnerWindow::operator=(SpawnerWindow &&other)
    {
        if (&other != this)
        {
            m_space = std::move(other.m_space);
            m_items = std::move(other.m_items);
            m_currentItem = std::move(other.m_currentItem);
            m_defaultGraphic = std::move(other.m_defaultGraphic);
            m_isPlacingItem = std::move(other.m_isPlacingItem);
        }

        return *this;
    }
}