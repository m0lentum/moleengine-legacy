#include <Graphics/AnimatedSprite.hpp>
//#include <iostream>

namespace me
{
	void AnimatedSprite::continuousUpdate(const sf::Time &timeElapsed)
	{
		if (m_isPlaying)
		{
			sf::Time frameDuration = m_anim.getDuration(m_currentFrame);

			m_currentTime += timeElapsed;
			if (m_currentTime >= frameDuration)
			{
				m_currentFrame++;
				m_currentTime -= frameDuration;
				if (m_currentFrame >= m_anim.getLength())
				{
					m_currentFrame = 0;
					if (--m_numLoops == 0)
						m_isPlaying = false;
				}

				m_textureRect = m_anim.getRect(m_currentFrame);
				updateVertices();
			}
		}
	}

	void AnimatedSprite::play(int loops)
	{
		m_currentFrame = 0;
		m_currentTime = sf::Time();
		m_isPlaying = true;
		m_numLoops = loops;
	}

	void AnimatedSprite::pause()
	{
		m_isPlaying = false;
	}

	void AnimatedSprite::resume()
	{
		m_isPlaying = true;
	}

	void AnimatedSprite::reset()
	{
		m_isPlaying = false;
		m_numLoops = -1;
		m_currentFrame = 0;
		m_currentTime = sf::Time();
	}

	void AnimatedSprite::jumpToFrame(int frame)
	{
		m_currentFrame = frame;
		m_currentTime = sf::Time();
	}

	int AnimatedSprite::getWidth() const
	{
		return m_size.x;
	}

	int AnimatedSprite::getHeight() const
	{
		return m_size.y;
	}

	void AnimatedSprite::setAnimation(const SpriteAnimationData &anim)
	{
		m_anim = anim;
	}


	void AnimatedSprite::initVertices()
	{
		m_vertices = sf::VertexArray(sf::TriangleFan, 4);
		m_vertices[0].position = sf::Vector2f(0, 0);
		m_vertices[1].position = sf::Vector2f(0, (float)m_size.y);
		m_vertices[2].position = sf::Vector2f((float)m_size.x, (float)m_size.y);
		m_vertices[3].position = sf::Vector2f((float)m_size.x, 0);
		updateVertices();
	}


	AnimatedSprite::AnimatedSprite(std::shared_ptr<sf::Texture> texture, const sf::Vector2i &startPosition, const sf::Vector2i &size,
		unsigned int numFrames, sf::Time frameDuration, bool playOnCreate) :
		m_size(size),
		m_currentFrame(0),
		m_numLoops(-1),
		m_isPlaying(playOnCreate)
	{
		m_texture = texture;
		m_textureRect = sf::IntRect(startPosition, size);
		m_anim.addStrip(startPosition, size, numFrames, frameDuration);
		initVertices();
	}

	AnimatedSprite::AnimatedSprite(std::shared_ptr<sf::Texture> texture, const SpriteAnimationData &anim, const sf::Vector2i &size, bool playOnCreate) :
		m_size(size),
		m_anim(anim),
		m_isPlaying(playOnCreate)
	{
		m_texture = texture;
		initVertices();
	}

	AnimatedSprite::AnimatedSprite(std::shared_ptr<sf::Texture> texture, const SpriteAnimationData &anim, bool playOnCreate) :
		m_isPlaying(playOnCreate)
	{
		//default size to size of first frame of animation
		sf::IntRect firstFrame = anim.getRect(0);
		AnimatedSprite(texture, anim, sf::Vector2i(firstFrame.width, firstFrame.height));
	}

	AnimatedSprite::AnimatedSprite(const AnimatedSprite &copy) :
		Graphic(copy),
		m_size(copy.m_size),
		m_currentFrame(copy.m_currentFrame),
		m_currentTime(copy.m_currentTime),
		m_numLoops(copy.m_numLoops),
		m_isPlaying(copy.m_isPlaying),
		m_anim(copy.m_anim)
	{
	}

	AnimatedSprite::AnimatedSprite(AnimatedSprite&& move) :
		Graphic(move),
		m_size(std::move(move.m_size)),
		m_currentFrame(std::move(move.m_currentFrame)),
		m_currentTime(std::move(move.m_currentTime)),
		m_numLoops(std::move(move.m_numLoops)),
		m_isPlaying(std::move(move.m_isPlaying)),
		m_anim(std::move(move.m_anim))
	{
	}

	AnimatedSprite& AnimatedSprite::operator=(AnimatedSprite&& other)
	{
		if (&other != this)
		{
			Graphic::operator=(std::move(other));
			m_size = std::move(other.m_size);
			m_currentFrame = std::move(other.m_currentFrame);
			m_currentTime = std::move(other.m_currentTime);
			m_numLoops = std::move(other.m_numLoops);
			m_isPlaying = std::move(other.m_isPlaying);
			m_anim = std::move(other.m_anim);
		}

		return *this;
	}

	AnimatedSprite::~AnimatedSprite()
	{
	}
}