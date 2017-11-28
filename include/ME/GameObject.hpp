#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include "IComponent.hpp"
#include <initializer_list>
#include <unordered_map>
#include <memory>

namespace me
{
	class Space;

	class GameObject : public sf::Transformable
	{
	private:
		/// The Space this object is in, if any.
		Space *m_space;

		std::unordered_map<std::string, std::unique_ptr<IComponent> > m_components;

		/// A unique identification string.
		const std::string m_id;
		/// Tracks how many game objects have been created. Used for generating a unique ID.
		static unsigned int numExisting;

	public:

		/// Store a pointer to the space containing this object
		void registerSpace(Space *space);
		/// Mark this object for destruction.
		/// It will be removed from the space it's in on the next fixedUpdate cycle.
		virtual void destroy();

		void addComponent(IComponent *component);
		void removeComponent(const std::string &type);
		IComponent * getComponent(const std::string &type) const;
		const std::unordered_map<std::string, std::unique_ptr<IComponent> > & getAllComponents() const;

		inline const std::string & getID() const { return m_id; }

		
		GameObject();
		/// Set a custom id (avoid using integers as this can break uniqueness)
		GameObject(const std::string &id);
		GameObject(std::initializer_list<IComponent*> components);
		GameObject(const std::string &id, std::initializer_list<IComponent*> components);
		GameObject(const GameObject &copy);

		virtual ~GameObject();
	};
}

#endif //GAME_OBJECT_HPP