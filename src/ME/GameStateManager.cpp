#include <GameStateManager.hpp>
//#include <iostream>

namespace me
{
	void GameStateManager::handleWindowEvent(const sf::Event &evt)
	{
		m_currentState->handleWindowEvent(evt);
	}

	void GameStateManager::continuousUpdate(const sf::Time &timeElapsed)
	{
		m_currentState->continuousUpdate(timeElapsed);
	}

	void GameStateManager::fixedUpdate()
	{
		m_currentState->fixedUpdate();
	}

	void GameStateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		m_currentState->draw(target, states);
	}

	GameStateManager::GameStateManager(AssetManager *assetManager) :
		m_currentState(NULL),
		m_assetManager(assetManager)
	{
	}

	GameStateManager::~GameStateManager()
	{
	}
}