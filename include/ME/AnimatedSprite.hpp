#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include <SFML/Graphics.hpp>

namespace me
{
	class AnimatedSprite : public sf::Drawable
	{
	public:

		void update();

		void play(int loops = -1);
		void pause() { isPlaying = false; }
		void resume() { isPlaying = true; }
		void reset();
		void jumpToFrame(int theFrame);

		int getWidth() const;
		int getHeight() const;

		AnimatedSprite(const sf::Texture &texture, const sf::Vector2i &startPosition, const sf::Vector2i &size,
			unsigned int numFrames = 1, unsigned int framesToSkip = 0);
		AnimatedSprite(const AnimatedSprite &copy);
		~AnimatedSprite();


	private:

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::Sprite sprite;
		const sf::Vector2i startPosition;
		const sf::Vector2i size;

		bool isPlaying;
		int numLoops;

		unsigned int numFrames;
		unsigned int currentFrame;
		unsigned int framesToSkip;
		unsigned int currentFramesSkipped;
	};
}


#endif //ANIMATED_SPRITE_HPP
