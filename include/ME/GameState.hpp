#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

namespace me
{
	class GameStateManager;

	/// An abstract base class for game states. 
	/// NOTE: You should #include GameStateManagement.hpp instead of this
	/// so you can use the *m_manager pointer to switch states
	class GameState
	{
	protected:
		GameStateManager *m_manager;

	public:
		/// Transition events are not mandatory and do nothing by default
		virtual void onTransitionIn() {}
		virtual void onTransitionOut() {}

		virtual void continuousUpdate(const sf::Time &timeElapsed) = 0;
		virtual void fixedUpdate() = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;


		inline void registerManager(GameStateManager *manager) { m_manager = manager; }
		inline GameStateManager* getManager() const { return m_manager; }


		virtual ~GameState() {}
	};
}


#endif //GAME_STATE_HPP