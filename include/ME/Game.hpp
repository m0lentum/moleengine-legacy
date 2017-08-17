#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "GameStateManager.hpp"

namespace me
{
	class Game
	{
	public:
		Game();

		virtual void begin();
		virtual void terminate();

	protected:
		sf::RenderWindow m_mainWindow;
		std::string m_title;
		sf::View m_view;
		sf::Clock m_clock;

		sf::Time m_fixedUpdateInterval; //TODO: have a separate settings class for this

		GameStateManager m_stateManager;


		void createWindow(unsigned int width, unsigned int height);

	private:
		bool m_isTerminated;
		sf::Time m_timeSinceFixedUpdate;


		void gameLoop();

		virtual void continuousUpdate(sf::Time timeElapsed) = 0;
		virtual void fixedUpdate() = 0;
		virtual void draw() = 0;

		void handleWindowEvents();
	};
}
#endif //GAME_HPP
