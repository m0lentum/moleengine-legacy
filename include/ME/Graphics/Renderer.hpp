#ifndef GRAPHIC_CONTROLLER_HPP
#define GRAPHIC_CONTROLLER_HPP

#include "../ISystem.hpp"
#include "Graphic.hpp"
#include "AnimatedSprite.hpp"
#include <vector>

namespace me
{
	class Renderer : public ISystem
	{
	public:

		virtual void continuousUpdate(sf::Time timeElapsed);
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


		virtual ~Renderer() {}
	};
}


#endif // GRAPHIC_CONTROLLER_HPP