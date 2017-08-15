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
		///Should be called once in each update loop. Handles frame transitions.
		void update(sf::Time timeElapsed);

		///Start the animation and play it for the specified number of loops
		void play(int loops = -1);
		///Pause the animation without resetting
		void pause() { m_isPlaying = false; }
		///Continue the animation from where it left off
		void resume() { m_isPlaying = true; }
		///Stop the animation and reset it to the first frame
		void reset();
		///Jump to the specified frame of animation
		void jumpToFrame(int frame);


		int getWidth() const;
		int getHeight() const;

		void setAnimation(const SpriteAnimationData &anim);


		///Constructor that creates an animation from a horizontal strip spritesheet
		AnimatedSprite(const sf::Texture *texture, const sf::Vector2i &startPosition, const sf::Vector2i &size,
			unsigned int numFrames, sf::Time frameDuration);
		///Constructor that only takes the texture to use (add frames with addFrame() or addStrip())
		AnimatedSprite(const sf::Texture *texture, const sf::Vector2i &size);
		///Constructor that takes a pre-created animation (not yet implemented)
		//AnimatedSprite(const sf::Texture *texture, const SpriteAnimationData &anim);
		///Copy constructor
		AnimatedSprite(const AnimatedSprite &copy);
		~AnimatedSprite();


	private:

		///Initialize the vertex array to draw
		void initVertices();
	};
}


#endif //ANIMATED_SPRITE_HPP
