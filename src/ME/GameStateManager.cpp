#include <GameStateManager.hpp>
//#include <iostream>

namespace me
{
	void GameStateManager::transitionTo(IGameState *state)
	{
		//transition events
		if (m_currentState)
			m_currentState->onTransitionOut();
		state->onTransitionIn();

		//switch pointers
		m_prevState = m_currentState;
		m_currentState = state;
	}

	void GameStateManager::backToPrevious()
	{
		if (m_prevState)
			transitionTo(m_prevState);
	}

	void GameStateManager::continuousUpdate(const sf::Time &timeElapsed)
	{
		if (m_currentState)
			m_currentState->continuousUpdate(timeElapsed);
	}

	void GameStateManager::fixedUpdate()
	{
		if (m_currentState)
			m_currentState->fixedUpdate();
	}

	void GameStateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		if (m_currentState)
			m_currentState->draw(target, states);
	}

	GameStateManager::GameStateManager() :
		m_currentState(NULL),
		m_prevState(NULL)
	{
	}

	GameStateManager::~GameStateManager()
	{
	}
}