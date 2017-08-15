#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "Graphic.hpp"
#include "SpriteAnimationData.hpp"

namespace me
{
	class AnimatedSprite : public me::Graphic
	{
	private:
		sf::Vector2i m_size;

		bool m_isPlaying;
		int m_numLoops; //number of loops left in current play

		unsigned int m_currentFrame;
		sf::Time m_currentTime; //time elapsed since last frame change

		//the animation data. TODO: allow multiple animations
		SpriteAnimationData m_anim;

	public:
		///Should be called once in each continuousUpdate loop of the game. Handles frame transitions.
		virtual void continuousUpdate(const sf::Time &timeElapsed);

		///Start the animation and play it for the specified number of loops
		void play(int loops = -1);
		///Pause the animation without resetting
		void pause();
		///Continue the animation from where it left off
		void resume();
		///Stop the animation and reset it to the first frame
		void reset();
		///Jump to the specified frame of animation
		void jumpToFrame(int frame);


		int getWidth() const;
		int getHeight() const;

		void setAnimation(const SpriteAnimationData &anim);


		///Constructor that generates an animation from a horizontal strip spritesheet
		AnimatedSprite(const sf::Texture *texture, const sf::Vector2i &startPosition, const sf::Vector2i &size,
			unsigned int numFrames, sf::Time frameDuration, bool playOnCreate = true);
		///Constructor that takes both a pre-created animation and a size (use in case animation has varying frame sizes)
		AnimatedSprite(const sf::Texture *texture, const SpriteAnimationData &anim, const sf::Vector2i &size, bool playOnCreate = true);
		///Constructor that takes a pre-created animation (assumes size based on first frame)
		AnimatedSprite(const sf::Texture *texture, const SpriteAnimationData &anim, bool playOnCreate = true);
		///Copy constructor
		AnimatedSprite(const AnimatedSprite &copy);
		~AnimatedSprite();


	private:

		///Initialize the vertex array to draw
		void initVertices();
	};
}


#endif //ANIMATED_SPRITE_HPP
