#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../ISystem.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Time.hpp>

namespace me
{
	class Graphic;

	class Renderer : public ISystem
	{
	public:

		virtual void continuousUpdate(sf::Time timeElapsed) override;
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

		void drawGraphic(Graphic *graphic, const sf::Transform &transform, sf::RenderTarget &target, sf::RenderStates states) const;

		virtual ~Renderer() {}
	};
}


#endif // RENDERER_HPP