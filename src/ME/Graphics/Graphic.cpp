#include <Graphics/Graphic.hpp>
#include <iostream>
#include <cmath>

namespace me
{
	void Graphic::continuousUpdate(const sf::Time &timeElapsed)
	{
		//Do nothing if there's no animation
	}

	void Graphic::fillWithColor(const sf::Color& color)
	{
		for (std::size_t i = 0; i < m_vertices.getVertexCount(); i++)
		{
			m_vertices[i].color = color;
		}
	}

	void Graphic::setTexture(std::shared_ptr<sf::Texture> texture, bool resetRect)
	{
		if (texture)
		{
			// Recompute the texture area if requested, or if there was no texture & rect before. Copied from sf::Shape.
			if (resetRect || (!m_texture && (m_textureRect == sf::IntRect())))
				setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));
		}

		m_texture = texture;
	}

	void Graphic::setTextureRect(const sf::IntRect &rect)
	{
		m_textureRect = rect;
		updateTexCoords();
	}

	void Graphic::setVertices(const sf::VertexArray &verts)
	{
		m_vertices = verts;
		updateVertices();
	}

	void Graphic::updateVertices()
	{
		m_bounds = m_vertices.getBounds();

		updateTexCoords();
		//TODO also update outline
	}

	void Graphic::updateTexCoords()
	{
		for (std::size_t i = 0; i < m_vertices.getVertexCount(); i++)
		{
			//straight out of sf::Shape source
			float xratio = m_bounds.width > 0 ? (m_vertices[i].position.x - m_bounds.left) / m_bounds.width : 0;
			float yratio = m_bounds.height > 0 ? (m_vertices[i].position.y - m_bounds.top) / m_bounds.height : 0;
			m_vertices[i].texCoords.x = m_textureRect.left + m_textureRect.width * xratio;
			m_vertices[i].texCoords.y = m_textureRect.top + m_textureRect.height * yratio;
		}
	}

	void Graphic::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = m_texture.get();

		target.draw(m_vertices, states);

		//TODO add outline
	}


	Graphic::Graphic() :
		m_texture(NULL),
		m_textureRect(),
		m_vertices(),
		m_bounds()
	{
	}

	Graphic::Graphic(const sf::VertexArray &vertices) :
		m_texture(NULL),
		m_textureRect(),
		m_vertices(vertices)
	{
		updateVertices();
	}

	Graphic::Graphic(std::shared_ptr<sf::Texture> texture, const sf::IntRect &textureRect, const sf::VertexArray &vertices) :
		m_texture(texture),
		m_textureRect(textureRect),
		m_vertices(vertices)
	{
		updateVertices();
	}

	Graphic::Graphic(const Graphic &copy) :
		m_texture(copy.m_texture),
		m_textureRect(copy.m_textureRect),
		m_vertices(copy.m_vertices),
		m_bounds(copy.m_bounds)
	{
	}

	Graphic::~Graphic()
	{
	}


	std::shared_ptr<Graphic> Graphic::makeCircle(float radius, unsigned int pointCount, const sf::Color &color)
	{
		sf::VertexArray verts(sf::PrimitiveType::TriangleFan, pointCount);

		const float angleIncrement = 6.2831853f / pointCount;

		float angle;
		for (unsigned int i = 0; i < pointCount; i++)
		{
			angle = i * angleIncrement;
			verts[i].position.x = std::cos(angle) * radius;
			verts[i].position.y = std::sin(angle) * radius;
			
			verts[i].color = color;
		}

		return std::make_shared<Graphic>(verts);
	}
}