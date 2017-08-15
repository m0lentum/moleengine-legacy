#include <Game.hpp>
#include <iostream>

namespace me
{
	void Game::handleWindowEvents()
	{
		sf::Event event;
		while (m_mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_mainWindow.close();
		}
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
			handleWindowEvents();

			sf::Time timeElapsed = m_clock.restart();

			continuousUpdate(timeElapsed);

			m_timeSinceFixedUpdate += timeElapsed;
			if (m_timeSinceFixedUpdate > m_fixedUpdateInterval)
			{
				m_timeSinceFixedUpdate -= m_fixedUpdateInterval;
				fixedUpdate();
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
		gameLoop();
	}

	void Game::terminate()
	{
		m_isTerminated = true;
	}

	Game::Game() :
		m_isTerminated(false)
	{
	}
}
