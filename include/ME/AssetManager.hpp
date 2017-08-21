#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <unordered_map>
#include <memory>
#include <SFML/Graphics/Texture.hpp>

namespace me
{
	/// Class for handling assets so they can be shared between game states
	class AssetManager
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;

	public:
		/// Load a texture from a file into memory
		void loadTexture(const std::string &path, const std::string &name);
		/// Get a pointer to a loaded texture
		std::shared_ptr<sf::Texture> getTexture(const std::string &name) const;

		/// Delete unused assets
		void cleanup();


		AssetManager();
		AssetManager(const AssetManager &copy) = delete; //disable copy constructor
		~AssetManager();
	};
}


#endif //ASSET_MANAGER_HPP