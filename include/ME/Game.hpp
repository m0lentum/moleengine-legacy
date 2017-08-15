#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

namespace me
{
	class Game
	{
	public:
		Game();

		void begin();
		void terminate();

	protected:
		sf::RenderWindow m_mainWindow;
		std::string m_title;
		sf::View m_view;
		sf::Clock m_clock;

		void createWindow(unsigned int width, unsigned int height);

	private:
		bool m_isTerminated;


		void gameLoop();

		virtual void update(sf::Time timeElapsed) = 0;
		virtual void draw() = 0;

		void handleWindowEvents();
	};
}
#endif //GAME_HPP
