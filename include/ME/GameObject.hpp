#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "Graphics/Graphic.hpp"
#include "IComponent.hpp"

namespace me
{
	class GameObject : public sf::Transformable, public IComponent
	{
	private:
		std::shared_ptr<Graphic> m_graphic;

	public:
		// IComponent update loops
		virtual void continuousUpdate(const sf::Time &timeElapsed);
		virtual void fixedUpdate();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


		void setGraphic(std::shared_ptr<Graphic> graphic);

		GameObject();
		/// Set the graphic upon initialization
		GameObject(std::shared_ptr<Graphic> graphic);
		GameObject(const GameObject &copy);

		virtual ~GameObject();
	};
}

#endif //GAME_OBJECT_HPP