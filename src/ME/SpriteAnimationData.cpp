#include <SpriteAnimationData.hpp>

namespace me
{
	void SpriteAnimationData::addFrame(const sf::IntRect rect, const sf::Time duration)
	{
		Frame frame;
		frame.rect = rect;
		frame.duration = duration;
		m_frames.push_back(frame);
	}

	void SpriteAnimationData::addFrame(const sf::IntRect rect)
	{
		addFrame(rect, m_defaultDuration);
	}

	void SpriteAnimationData::addStrip(const sf::Vector2i &startPosition, const sf::Vector2i &size,
		unsigned int numFrames, sf::Time frameDuration)
	{
		for (unsigned int i = 0; i < numFrames; i++)
		{
			addFrame(sf::IntRect(sf::Vector2i(startPosition.x + i * size.x, startPosition.y), size), frameDuration);
		}
	}

	void SpriteAnimationData::setDefaultDuration(const sf::Time duration)
	{
		m_defaultDuration = duration;
	}

	sf::IntRect SpriteAnimationData::getRect(int index)
	{
		return m_frames[index].rect;
	}

	sf::Time SpriteAnimationData::getDuration(int index)
	{
		return m_frames[index].duration;
	}

	unsigned int SpriteAnimationData::getLength()
	{
		return m_frames.size();
	}

	SpriteAnimationData::SpriteAnimationData()
	{
		m_defaultDuration = sf::milliseconds(17); //60fps
		//TODO: have a config file to read actual framerate from
	}

	SpriteAnimationData::SpriteAnimationData(const SpriteAnimationData& copy) :
		m_frames(copy.m_frames),
		m_defaultDuration(copy.m_defaultDuration)
	{
	}

	SpriteAnimationData::~SpriteAnimationData()
	{
	}
}