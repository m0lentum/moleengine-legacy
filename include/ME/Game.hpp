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

		sf::RenderWindow mainWindow;
		std::string title;
		sf::View view;

		void createWindow(unsigned int width, unsigned int height);

	private:

		bool isTerminated;

		void gameLoop();

		virtual void update() = 0;
		virtual void draw() = 0;

		void handleWindowEvents();
	};
}
#endif //GAME_HPP
