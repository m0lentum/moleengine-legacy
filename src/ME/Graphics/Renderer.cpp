#include <Graphics/Renderer.hpp>
#include <Graphics/Graphic.hpp>
#include <Graphics/AnimatedSprite.hpp>
#include <Space.hpp>
#include <ComponentStorageUnit.hpp>
#include <GameObject.hpp>

namespace me
{
	void Renderer::continuousUpdate(sf::Time timeElapsed)
	{
		m_space->each<AnimatedSprite>([&](ComponentStorageUnit<AnimatedSprite> &unit)
		{
			unit.getComponent()->continuousUpdate(timeElapsed);
		});
	}

	void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		m_space->each<Graphic>([&](ComponentStorageUnit<Graphic> &unit)
		{
			drawGraphic(unit.getComponent(), unit.getParent()->getTransform(), target, states);
		});

		m_space->each<AnimatedSprite>([&](ComponentStorageUnit<AnimatedSprite> &unit)
		{
			drawGraphic(unit.getComponent(), unit.getParent()->getTransform(), target, states);
		});
	}

	void Renderer::drawGraphic(Graphic *graphic, const sf::Transform &transform, sf::RenderTarget &target, sf::RenderStates states) const
	{
		states.transform *= transform;
		graphic->draw(target, states);
	}
}