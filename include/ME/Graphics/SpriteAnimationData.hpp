#ifndef ANIMATION_DATA_HPP
#define ANIMATION_DATA_HPP

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

namespace me
{
	/// Class to hold information specific to a single spritesheet animation loop.
	class SpriteAnimationData
	{
	private:
		struct Frame
		{
			sf::IntRect rect; //area in texture
			sf::Time duration; //time to stay on the frame
		};

		std::vector<Frame> m_frames; //all frames of the animation
		sf::Time m_defaultDuration; //default duration of frame

	public:
		/// Add a frame with the specified duration
		void addFrame(sf::IntRect rect, sf::Time duration);
		/// Add a frame with the default duration
		void addFrame(sf::IntRect rect);

		/// Add a horizontal strip containing multiple frames with the specified duration
		void addStrip(const sf::Vector2i &startPosition, const sf::Vector2i &size,
			unsigned int numFrames, sf::Time frameDuration);

		void setDefaultDuration(sf::Time duration);

		/// Rect of frame specified by index
		sf::IntRect getRect(unsigned int index) const;
		/// Duration of frame specified by index
		sf::Time getDuration(unsigned int index) const;
		/// Number of frames in the animation
		unsigned int length() const;


		SpriteAnimationData();
		SpriteAnimationData(const SpriteAnimationData& copy);
		~SpriteAnimationData();
	};
}



#endif //ANIMATION_DATA_HPP