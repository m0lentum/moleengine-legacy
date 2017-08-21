#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include "AssetManager.hpp"

namespace me
{
	class GameStateManager;

	/// An abstract base class for game states. 
	/// NOTE: You should #include GameStateManagement.hpp instead of this
	/// so you can use the *m_manager pointer to switch states
	class GameState
	{
	protected:
		GameStateManager *m_stateManager;
		AssetManager *m_assetManager;

	public:
		/// Transition events are not mandatory and do nothing by default
		virtual void onTransitionIn() {}
		virtual void onTransitionOut() {}

		virtual void continuousUpdate(const sf::Time &timeElapsed) = 0;
		virtual void fixedUpdate() = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;


		inline void registerStateManager(GameStateManager *manager) { m_stateManager = manager; }
		inline GameStateManager* getStateManager() const { return m_stateManager; }
		inline void registerAssetManager(AssetManager *manager) { m_assetManager = manager; }
		inline AssetManager* getAssetManager() const { return m_assetManager; }

		virtual ~GameState() {}
	};
}


#endif //GAME_STATE_HPP