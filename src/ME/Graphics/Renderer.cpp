#include <Graphics/Renderer.hpp>
#include <Graphics/Graphic.hpp>
#include <Graphics/AnimatedSprite.hpp>
#include <Space.hpp>
#include <Component.hpp>
#include <GameObject.hpp>

namespace me
{
	void Renderer::continuousUpdate(sf::Time timeElapsed)
	{
		m_space->each<AnimatedSprite>([&](Component<AnimatedSprite> &comp)
		{
			comp.getComponent()->continuousUpdate(timeElapsed);
		});
	}

	void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		m_space->each<Graphic>([&](Component<Graphic> &comp)
		{
			drawGraphic(comp.getComponent(), comp.getParent()->getTransform(), target, states);
		});

		m_space->each<AnimatedSprite>([&](Component<AnimatedSprite> &comp)
		{
			drawGraphic(comp.getComponent(), comp.getParent()->getTransform(), target, states);
		});
	}

	void Renderer::drawGraphic(Graphic *graphic, const sf::Transform &transform, sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= transform;
		graphic->draw(target, states);
	}
}