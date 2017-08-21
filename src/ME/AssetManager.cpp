#include <AssetManager.hpp>
//#include <iostream>

namespace me
{
	void AssetManager::loadTexture(const std::string &path, const std::string &name)
	{
		//Only do anything if the name is unique
		if (m_textures.count(name) <= 0)
		{
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			texture->loadFromFile(path);
			m_textures[name] = texture;
		}
	}

	std::shared_ptr<sf::Texture> AssetManager::getTexture(const std::string &name) const
	{
		return m_textures.at(name); //this will throw an exception if the key doesn't exist
	}

	void AssetManager::cleanup()
	{
		//loop through all textures and see if they're referenced anywhere
		for (auto iter = m_textures.begin(); iter != m_textures.end(); iter++)
		{
			if (iter->second.use_count() <= 1)
				m_textures.erase(iter);
		}
	}


	AssetManager::AssetManager()
	{
	}
	
	AssetManager::~AssetManager()
	{
	}
}