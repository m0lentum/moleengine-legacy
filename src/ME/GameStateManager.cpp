#include <GameStateManager.hpp>
//#include <iostream>

namespace me
{
	void GameStateManager::addState(const std::string &key, GameState *state)
	{
		m_states[key] = std::shared_ptr<GameState>(state);
		state->registerManager(this);
	}

	void GameStateManager::transitionTo(const std::string &key)
	{
		GameState *state = m_states.at(key).get(); //this will throw an exception if the key doesn't exist

		if (m_currentState) m_currentState->onTransitionOut(); //transition events
		state->onTransitionIn();

		m_currentState = state;
	}

	void GameStateManager::continuousUpdate(const sf::Time &timeElapsed)
	{
		if (m_currentState) m_currentState->continuousUpdate(timeElapsed);
	}

	void GameStateManager::fixedUpdate()
	{
		if (m_currentState) m_currentState->fixedUpdate();
	}

	void GameStateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		if (m_currentState) m_currentState->draw(target, states);
	}

	GameStateManager::GameStateManager() :
		m_currentState(NULL) //I don't know why this is necessary but it seems m_currentState gets initialized to something else without it
	{
	}

	GameStateManager::~GameStateManager()
	{
	}
}