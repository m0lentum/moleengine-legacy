#include <ME/Game.hpp>


namespace me
{
	void Game::handleWindowEvents()
	{
		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();
		}
	}

	void Game::gameLoop()
	{
		while (!isTerminated)
		{
			if (!mainWindow.isOpen())
			{
				terminate();
				continue;
			}
			handleWindowEvents();
			update();
			draw();
		}
	}

	void Game::createWindow(unsigned int width, unsigned int height)
	{
		mainWindow.create(sf::VideoMode(width, height), title, sf::Style::Close);
		view.setSize((float)width, (float)height);
	}

	void Game::begin()
	{
		gameLoop();
	}

	void Game::terminate()
	{
		isTerminated = true;
	}

	Game::Game() :
		isTerminated(false)
	{
	}
}
