#include <Devtools/SpawnerWindow.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <GameObject.hpp>
#include <iostream>

namespace me
{
    void SpawnerWindow::continuousUpdate(sf::Time timeElapsed)
    {
        ImGui::Begin("Spawner");

        for (auto &item : m_items)
        {
            if (ImGui::Button(item.name.c_str()))
            {
                GameObject *obj = item.createFunction(m_space);
                
                std::cout << "Spawning a " << item.name << std::endl;
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

    SpawnerWindow::SpawnerWindow(Space *space) :
        m_space(space)
    {
    }

    SpawnerWindow::SpawnerWindow(SpawnerWindow &copy) :
        m_space(copy.m_space),
        m_items(copy.m_items),
        m_defaultGraphic(copy.m_defaultGraphic)
    {
    }
    
    SpawnerWindow::SpawnerWindow(SpawnerWindow &&move) :
        m_space(std::move(move.m_space)),
        m_items(std::move(move.m_items)),
        m_defaultGraphic(std::move(move.m_defaultGraphic))
    {
    }
    
    SpawnerWindow::~SpawnerWindow() {}

    SpawnerWindow& SpawnerWindow::operator=(SpawnerWindow &&other)
    {
        if (&other != this)
        {
            m_space = std::move(other.m_space);
            m_items = std::move(other.m_items);
            m_defaultGraphic = std::move(other.m_defaultGraphic);
        }

        return *this;
    }
}