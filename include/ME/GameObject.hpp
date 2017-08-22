#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "Graphics/Graphic.hpp"

namespace me
{
	class GameObject : public sf::Transformable, public sf::Drawable
	{
	private:
		std::shared_ptr<Graphic> m_graphic;

	public:
		virtual void continuousUpdate(const sf::Time &timeElapsed);
		virtual void fixedUpdate();

		void setGraphic(std::shared_ptr<Graphic> graphic);

		GameObject();
		GameObject(std::shared_ptr<Graphic> graphic);
		GameObject(const GameObject &copy);
		virtual ~GameObject();

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	};
}

#endif //GAME_OBJECT_HPP