#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "../ISystem.hpp"
#include <SFML/System/Vector2.hpp>

namespace me
{
	class Physics : public ISystem
	{
	private:

		sf::Vector2f m_gravity;

	public:

		virtual void fixedUpdate() override;

	private:

		void applyMovement();

		void checkCollisions();

	public:

		Physics();
		Physics(const sf::Vector2f &gravity);

		virtual ~Physics();
	};
}

#endif // PHYSICS_HPP