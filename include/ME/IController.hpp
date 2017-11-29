#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <string>

namespace me
{
	class Space;
	class IComponent;

	/// Controllers work within Spaces and control a specific Component.
	class IController
	{
	protected:

		Space *m_space;

	public:

		// update loops
		virtual void continuousUpdate(const sf::Time &timeElapsed) {}
		virtual void fixedUpdate() {}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) {}

		/// The type of the Controller corresponds to the type of Components it controls.
		virtual std::string getType() = 0;

		/// The process of storing and removing Components must be individually implemented by each Controller
		/// due to type casts needed to access the Component's special features
		virtual void registerComponent(IComponent *component) = 0;
		virtual void removeComponent(std::string id) = 0;


		inline void registerSpace(Space *space) { m_space = space; }

		virtual ~IController() {}
	};
}


#endif // ICONTROLLER_HPP