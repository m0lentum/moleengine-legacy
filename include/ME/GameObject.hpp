#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <initializer_list>
#include <vector>
#include <memory>

namespace me
{
	class Space;
	class IBehavior;

	class GameObject : public sf::Transformable
	{
	private:
		/// The Space this object is in, if any.
		Space *m_space;

		std::vector<std::unique_ptr<IBehavior> > m_behaviors;

		/// A unique identification number.
		const unsigned int m_id;
		/// Tracks how many game objects have been created. Used for generating a unique ID.
		static unsigned int numExisting;

	public:

		/// Store a pointer to the space containing this object
		void registerSpace(Space *space);
		/// Mark this object for destruction.
		/// It will be removed from the space it's in on the next fixedUpdate cycle.
		virtual void destroy();

		void addBehavior(IBehavior *behavior);

		inline const unsigned int getID() const { return m_id; }

		/// continuousUpdate all behaviors
		void continuousUpdate(sf::Time timeElapsed);
		/// fixedUpdate all behaviors
		void fixedUpdate();
		/// draw all behaviors
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

		
		GameObject();
		GameObject(std::initializer_list<IBehavior*> behaviors);
		GameObject(const GameObject &copy);

		virtual ~GameObject();
	};
}

#endif //GAME_OBJECT_HPP