#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "AnimatedSprite.hpp"

//TODO this will have to be entirely reworked for use with Graphic

namespace me
{
	class GameObject : public sf::Transformable, public sf::Drawable
	{
	public:

		virtual void continuousUpdate() = 0;

	private:

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	};


	class GameObjectAnimated : public GameObject
	{
	public:

		void continuousUpdate();

		void setGraphic(const me::AnimatedSprite &theGraphic);

		GameObjectAnimated(const GameObjectAnimated &copy);
		GameObjectAnimated(const me::AnimatedSprite &graphic);

	private:

		std::unique_ptr<me::AnimatedSprite> graphic;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};


	class GameObjectStatic : public GameObject
	{
	public:

		void continuousUpdate();

		void setGraphic(sf::Drawable *theGraphic);

		GameObjectStatic(GameObjectStatic &copy);
		GameObjectStatic(sf::Drawable *graphic);

	private:

		std::unique_ptr<sf::Drawable> graphic;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}

#endif //GAME_OBJECT_HPP