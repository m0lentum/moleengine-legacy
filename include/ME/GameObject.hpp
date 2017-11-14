#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "Graphics/Graphic.hpp"
#include "IComponent.hpp"

namespace me
{
	class Space;

	class GameObject : public sf::Transformable, public IComponent
	{
	protected:
		/// The Space this object is in, if any.
		Space *m_space;

		std::unique_ptr<Graphic> m_graphic;

	public:
		// IComponent update loops
		virtual void continuousUpdate(const sf::Time &timeElapsed);
		virtual void fixedUpdate();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		/// Store a pointer to the space containing this object
		void registerSpace(Space *space);
		/// Mark this object for destruction.
		/// It will be removed from the space it's in on the next fixedUpdate cycle.
		virtual void destroy();

		void setGraphic(Graphic *graphic);

		GameObject();
		/// Set the graphic upon initialization
		GameObject(Graphic *graphic);
		GameObject(const GameObject &copy);

		virtual ~GameObject();
	};
}

#endif //GAME_OBJECT_HPP