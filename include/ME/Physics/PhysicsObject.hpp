#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "../GameObject.hpp"
#include <SFML/System/Vector2.hpp>

namespace me
{
	class PhysicsObject : public GameObject
	{
	public: 
		struct Props
		{
			bool isStatic = false; // if true, object will not move during physics checks
			bool isSolid = true; // if false, object won't be checked for collisions
			bool isKinematic = false; // if true, object will not be affected by collisions

			sf::Vector2f velocity;
			float mass = 1.0f;
			float elasticity = 0.75f;
			float friction = 0.05f;
			float drag = 0.02f;
			// TODO: angular velocity and related variables
		};
	protected:
		Props m_props;

	public:
		inline const Props* getProps() const { return &m_props; }

		/// Apply a force to this object, meaning mass and (TODO) point of impact will be considered.
		void applyForce(const sf::Vector2f &force);
		/// Add a vector to the velocity of the object without regard to physical properties (used for gravity).
		inline void accelerate(const sf::Vector2f &acc) { m_props.velocity += acc; }
		/// Set the velocity to the given value without regard to physical properties.
		inline void setVelocity(const sf::Vector2f &vel) { m_props.velocity = vel; }

		// This needs to be re-implemented here in order for Space::removeObject to remove the object from the right container
		virtual void destroy();

		/// Sets props to default values and leaves the graphic null.
		PhysicsObject();
		/// Sets props to given values and leaves the graphic null.
		PhysicsObject(const Props &props);
		/// Sets props to default values and sets the graphic.
		PhysicsObject(std::shared_ptr<Graphic> graphic);
		/// Sets props to given values and sets the graphic.
		PhysicsObject(const Props &props, std::shared_ptr<Graphic> graphic);
		/// Copy constructor.
		PhysicsObject(const PhysicsObject &copy);
		virtual ~PhysicsObject();
	};
}


#endif // PHYSICS_OBJECT_HPP