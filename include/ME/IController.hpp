#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <unordered_map>
#include <memory>

namespace me
{
	class Space;
	class IComponent;

	/// Controllers work within Spaces and control a specific Component.
	class IController
	{
	protected:

		Space *m_parentSpace;
		std::unordered_map<unsigned int, IComponent* > m_components;

	public:

		// update loops
		virtual void continuousUpdate(const sf::Time &timeElapsed) {}
		virtual void fixedUpdate() {}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) {}

		/// The type of the Controller corresponds to the type of Components it controls.
		virtual std::string getType() = 0;


		virtual void registerComponent(IComponent *component);
		virtual void removeComponent(unsigned int id);


		void registerSpace(Space *space);

		virtual ~IController() {}
	};
}


#endif // ICONTROLLER_HPP