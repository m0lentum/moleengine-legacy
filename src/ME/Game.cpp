#include <Game.hpp>
#include "Input/Keyboard.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"
#include <iostream>

namespace me
{
	void Game::handleWindowEvents()
	{
		sf::Event event;
		while (m_mainWindow.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				m_mainWindow.close();
			else
				handleWindowEvent(event);
		}
	}

	void Game::handleWindowEvent(sf::Event &evt)
	{
		m_stateManager.handleWindowEvent(evt);
	}

	void Game::gameLoop()
	{
		while (!m_isTerminated)
		{
			if (!m_mainWindow.isOpen())
			{
				terminate();
				continue;
			}
			
			sf::Time timeElapsed = m_clock.restart();

			continuousUpdate(timeElapsed);

			m_timeSinceFixedUpdate += timeElapsed;
			if (m_timeSinceFixedUpdate > m_fixedUpdateInterval)
			{
				m_timeSinceFixedUpdate -= m_fixedUpdateInterval;
				handleWindowEvents();				
				fixedUpdate();

				Keyboard::updateTracked();
			}

			draw();
		}
	}

	void Game::createWindow(unsigned int width, unsigned int height)
	{
		m_mainWindow.create(sf::VideoMode(width, height), m_title, sf::Style::Close);
		m_view.setSize((float)width, (float)height);
	}

	void Game::begin()
	{
		init();
		ImGui::SFML::Init(m_mainWindow);
		gameLoop();
	}

	void Game::terminate()
	{
		m_isTerminated = true;
	}

	Game::Game() :
		m_isTerminated(false),
		m_stateManager(&m_assetManager)
	{
	}
}
