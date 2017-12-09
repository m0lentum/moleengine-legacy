#ifndef GAME_STATE_MANAGER_HPP
#define GAME_STATE_MANAGER_HPP

#include <unordered_map>
#include <memory>
#include "IGameState.hpp"
#include <typeindex>
#include <iostream>

namespace me
{
	class AssetManager;

	/// Keeps track of the currently active game state.
	/// Primarily used to direct update loop calls to the right state.
	class GameStateManager 
	{
	private:

		AssetManager *m_assetManager;

		IGameState *m_currentState;
		std::unordered_map<std::type_index, std::unique_ptr<IGameState> > m_states;

	public:

		template <typename T>
		void transitionTo();

		template <typename T, typename... Args>
		typename std::enable_if<std::is_base_of<IGameState, T>::value, T*>::type
			createState(Args&&... args);
		

		void continuousUpdate(const sf::Time &timeElapsed);
		void fixedUpdate();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		
		GameStateManager(AssetManager *assetManager);
		~GameStateManager();
	};




	template <typename T>
	void GameStateManager::transitionTo()
	{
		std::type_index index(typeid(T));

		if (m_states.count(index) > 0)
		{
			m_currentState->onTransitionOut();

			m_currentState = m_states.at(index).get();

			m_currentState->onTransitionIn();
		}
	}

	template <typename T, typename... Args>
	typename std::enable_if<std::is_base_of<IGameState, T>::value, T*>::type
		GameStateManager::createState(Args&&... args)
	{
		std::type_index index(typeid(T));

		T *state = new T(args...);
		state->registerAssetManager(m_assetManager);
		state->registerStateManager(this);

		m_states[index] = std::unique_ptr<T>(state);

		if (!m_currentState)
		{
			m_currentState = state;
			m_currentState->onTransitionIn();
		}

		return state;
	}
}


#endif //GAME_STATE_MANAGER_HPP