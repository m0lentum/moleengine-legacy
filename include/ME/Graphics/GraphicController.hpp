#ifndef GRAPHIC_CONTROLLER_HPP
#define GRAPHIC_CONTROLLER_HPP

#include "../IController.hpp"
#include "Graphic.hpp"
#include "AnimatedSprite.hpp"
#include <vector>

namespace me
{
	class GraphicController : public IController
	{
	public:

		virtual void continuousUpdate(sf::Time timeElapsed);
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


		virtual ~GraphicController();
	};
}


#endif // GRAPHIC_CONTROLLER_HPP