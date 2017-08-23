#ifndef GAME_STATE_MANAGER_HPP
#define GAME_STATE_MANAGER_HPP

#include <unordered_map>
#include <memory>
#include "IGameState.hpp"
#include "IComponent.hpp"

namespace me
{
	/// Keeps track of the currently active game state.
	/// Primarily used to direct update loop calls to the right state.
	class GameStateManager : public IComponent
	{
	private:
		/// Pointer to currently active state
		IGameState *m_currentState;
		/// Pointer to previously active state
		IGameState *m_prevState;

	public:
		/// Switch to a different state
		void transitionTo(IGameState *state);
		/// Switch back to the previously active state
		void backToPrevious();

		// IComponent update loops
		virtual void continuousUpdate(const sf::Time &timeElapsed);
		virtual void fixedUpdate();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		
		GameStateManager();
		~GameStateManager();
	};
}


#endif //GAME_STATE_MANAGER_HPP