#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "../ISystem.hpp"
#include <SFML/System/Vector2.hpp>
#include <unordered_map>

namespace me
{
	struct Contact;

	const int defaultSolverIterations = 4;
	// how much of the penetration of two objects should be resolved each frame (0..1)
	const float penResolveFactor = 0.8f;

	class Physics : public ISystem
	{
	private:

		sf::Vector2f m_gravity;

		// contacts are stored between frames to make use of temporal coherence in collision detection
		std::unordered_map<long int, Contact> m_contacts;
		int m_solverIterations;

	public:

		virtual void fixedUpdate() override;

		inline void setSolverIterations(int count) { m_solverIterations = count; }

	private:

		void applyMovement();

		void findContacts();
		void solveContacts();

		/// Contacts are identified by a single long 32-bit int composed of the two contacting objects'
		/// IDs with the lower one shifted to the left 16 bits
		static long int contactID(int id1, int id2);

	public:

		Physics();
		Physics(const sf::Vector2f &gravity);

		virtual ~Physics();
	};
}

#endif // PHYSICS_HPP