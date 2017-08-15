#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "Graphic.hpp"

namespace me
{
	class GameObject : public sf::Transformable, public sf::Drawable
	{
	private:
		std::shared_ptr<Graphic> m_graphic;

	public:
		virtual void continuousUpdate(const sf::Time &timeElapsed);
		virtual void fixedUpdate();

		void setGraphic(Graphic *graphic);

		GameObject();
		GameObject(Graphic *graphic);
		GameObject(const GameObject &copy);
		~GameObject();

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	};
}

#endif //GAME_OBJECT_HPP