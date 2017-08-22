#include <AssetManager.hpp>
#include <stdexcept>
#include <iostream>

namespace me
{
	void AssetManager::loadTexture(const std::string &path, const std::string &name)
	{
		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
		texture->loadFromFile(path);
		m_textures[name] = texture;
	}

	std::shared_ptr<sf::Texture> AssetManager::getTexture(const std::string &name) const
	{
		try
		{
			return m_textures.at(name);
		}
		catch (const std::out_of_range&)
		{
			std::cerr << "Error: attempted to get a texture that doesn't exist: " << name << std::endl;
			return NULL;
		}
	}

	void AssetManager::cleanup()
	{
		//loop through all textures and see if they're referenced anywhere, delete if not
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