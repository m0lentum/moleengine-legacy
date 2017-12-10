#include <Physics/ColliderCircle.hpp>
#include <Physics/CollisionChecker.hpp>
#include <Graphics/Graphic.hpp>

namespace me
{
	const float ColliderCircle::getRadius() const
	{
		return m_radius;
	}


	
	sf::VertexArray ColliderCircle::toVertexArray(const sf::Color &color) const
	{
		return Graphic::makeCircle(m_radius, 25, color);
	}


	ColliderCircle::ColliderCircle(float radius) :
		m_radius(radius)
	{
	}

	ColliderCircle::ColliderCircle(const ColliderCircle &copy) :
		m_radius(copy.m_radius)
	{
	}

	ColliderCircle::ColliderCircle(ColliderCircle&& move) :
		m_radius(std::move(move.m_radius))
	{
	}

	ColliderCircle& ColliderCircle::operator=(ColliderCircle&& other)
	{
		if (this != &other)
		{
			m_radius = std::move(other.m_radius);
		}

		return *this;
	}

	ColliderCircle::~ColliderCircle()
	{
	}
}