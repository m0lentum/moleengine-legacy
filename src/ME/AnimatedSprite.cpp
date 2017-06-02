#include <ME/AnimatedSprite.hpp>

namespace me
{
	void AnimatedSprite::update()
	{
		++currentFramesSkipped;
		if (isPlaying)
		{
			if (currentFramesSkipped >= framesToSkip)
			{
				++currentFrame;
				currentFramesSkipped = 0;
				if (currentFrame >= numFrames)
				{
					currentFrame = 0;
					if (--numLoops == 0)
						isPlaying = false;
				}

				sprite.setTextureRect(sf::IntRect(sf::Vector2i
					(currentFrame * size.x + startPosition.x, startPosition.y), size));
			}
		}
	}

	void AnimatedSprite::play(int loops)
	{
		currentFrame = 0;
		currentFramesSkipped = 0;
		isPlaying = true;
		numLoops = loops;
	}

	void AnimatedSprite::reset()
	{
		isPlaying = false;
		numLoops = -1;
		currentFrame = 0;
		currentFramesSkipped = 0;
	}

	void AnimatedSprite::jumpToFrame(int theFrame)
	{
		currentFrame = theFrame;
		currentFramesSkipped = 0;
	}

	int AnimatedSprite::getWidth() const
	{
		return size.x;
	}

	int AnimatedSprite::getHeight() const
	{
		return size.y;
	}

	void AnimatedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}

	AnimatedSprite::AnimatedSprite(const sf::Texture &texture, const sf::Vector2i &startPosition, const sf::Vector2i &size,
		unsigned int numFrames, unsigned int framesToSkip) :
		sprite(texture, sf::IntRect(startPosition, size)),
		startPosition(startPosition),
		size(size),
		numFrames(numFrames),
		framesToSkip(framesToSkip),
		currentFrame(0),
		currentFramesSkipped(0),
		numLoops(-1),
		isPlaying(true)
	{
		
	}

	AnimatedSprite::AnimatedSprite(const AnimatedSprite &copy) :
		sprite(copy.sprite),
		startPosition(copy.startPosition),
		size(copy.size),
		numFrames(copy.numFrames),
		framesToSkip(copy.framesToSkip),
		currentFrame(copy.currentFrame),
		currentFramesSkipped(copy.currentFramesSkipped),
		numLoops(copy.numLoops),
		isPlaying(copy.isPlaying)
	{
	}

	AnimatedSprite::~AnimatedSprite()
	{
	}
}