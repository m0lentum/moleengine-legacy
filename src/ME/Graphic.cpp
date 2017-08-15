#include <Graphic.hpp>
#include <iostream>

namespace me
{
	void Graphic::fillWithColor(const sf::Color& color)
	{
		for (std::size_t i = 0; i < m_vertices.getVertexCount(); i++)
		{
			m_vertices[i].color = color;
		}
	}

	void Graphic::updateVertices()
	{
		m_bounds = m_vertices.getBounds();

		updateTexCoords();
		//TODO also continuousUpdate outline
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
		states.texture = m_texture;

		target.draw(m_vertices, states);

		//TODO add outline
	}


	Graphic::Graphic() :
		m_texture(NULL),
		m_textureRect(),
		m_vertices(sf::TriangleFan),
		m_bounds()
	{
	}

	Graphic::~Graphic()
	{
	}
}