#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "GameStateManager.hpp"
#include "AssetManager.hpp"

namespace me
{
	class Game
	{
	public:
		Game();

		/// Do all the initial setup (create windows and setup states etc.) here.
		/// Alternatively, states could be created in the constructor.
		/// Call Game::begin() at the end of your override to start the game loop.
		virtual void begin();
		/// Make the game close itself.
		virtual void terminate();

	protected:
		sf::RenderWindow m_mainWindow;
		std::string m_title;
		sf::View m_view;
		sf::Clock m_clock;

		sf::Time m_fixedUpdateInterval; //TODO: have a separate settings class for this

		GameStateManager m_stateManager;
		AssetManager m_assetManager;

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
