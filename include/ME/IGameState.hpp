#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "AssetManager.hpp"
#include "IComponent.hpp"

namespace me
{
	class GameStateManager;

	/// An abstract base class for game states. 
	/// NOTE: You should #include GameStateManagement.hpp instead of this
	/// so you can use the *m_stateManager pointer to switch states
	class IGameState : public IComponent
	{
	protected:
		GameStateManager *m_stateManager;
		AssetManager *m_assetManager;

	public:
		/// Transition events are not mandatory and do nothing by default
		virtual void onTransitionIn() {}
		virtual void onTransitionOut() {}

		// IComponent update loops inherited

		inline void registerStateManager(GameStateManager *manager) { m_stateManager = manager; }
		inline GameStateManager* getStateManager() const { return m_stateManager; }
		inline void registerAssetManager(AssetManager *manager) { m_assetManager = manager; }
		inline AssetManager* getAssetManager() const { return m_assetManager; }


		virtual ~IGameState() {}
	};
}


#endif //GAME_STATE_HPP