#include <Graphics/Renderer.hpp>
#include <iostream>
namespace me
{
	void Renderer::continuousUpdate(sf::Time timeElapsed)
	{

	}

	void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		std::cout << "Hell yea" << std::endl;
	}
}