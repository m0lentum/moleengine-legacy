#ifndef GAME_STATE_MANAGER_HPP
#define GAME_STATE_MANAGER_HPP

#include <unordered_map>
#include <memory>
#include "GameState.hpp"

namespace me
{
	class Game;

	class GameStateManager
	{
	private:
		/// States mapped to strings
		std::unordered_map<std::string, std::shared_ptr<GameState>> m_states;
		/// Pointer to currently active state so we don't have to look it up in the map every frame
		GameState *m_currentState;
		/// Pointer to the game which owns this object
		Game *m_game;

	public:
		/// Add a new state to the map
		void addState(const std::string &key, GameState *state);
		std::shared_ptr<GameState> getState(const std::string &key) const;
		/// Switch to a different state
		void transitionTo(const std::string &key);


		void continuousUpdate(const sf::Time &timeElapsed);
		void fixedUpdate();
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;


		inline void registerGame(Game *const game) { m_game = game; }
		inline Game* getGame() const { return m_game; }

		
		GameStateManager();
		~GameStateManager();
	};
}


#endif //GAME_STATE_MANAGER_HPP