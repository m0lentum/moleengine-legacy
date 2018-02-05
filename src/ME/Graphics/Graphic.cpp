#include <Graphics/Graphic.hpp>
#include <iostream>
#include <cmath>
#include <Physics/VectorMath.hpp>

namespace me
{
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

	Graphic::Graphic(Graphic&& move) :
		m_texture(std::move(move.m_texture)),
		m_textureRect(std::move(move.m_textureRect)),
		m_vertices(std::move(move.m_vertices)),
		m_bounds(std::move(move.m_bounds))
	{
	}

	Graphic::~Graphic()
	{
	}


	Graphic& Graphic::operator=(Graphic&& other)
	{
		if (this != &other)
		{
			m_texture = std::move(other.m_texture);
			m_textureRect = std::move(other.m_textureRect);
			m_vertices = std::move(other.m_vertices);
			m_bounds = std::move(other.m_bounds);
		}

		return *this;
	}


	sf::VertexArray Graphic::makeCircle(float radius, unsigned int pointCount, const sf::Color &color)
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

		return verts;
	}

	sf::VertexArray Graphic::makeRect(float width, float height, const sf::Color &color)
	{
		sf::VertexArray verts(sf::PrimitiveType::TriangleFan, 4);

		float hw = width / 2;
		float hh = height / 2;

		verts[0].position = sf::Vector2f(-hw, -hh);
		verts[1].position = sf::Vector2f(-hw, hh);
		verts[2].position = sf::Vector2f(hw, hh);
		verts[3].position = sf::Vector2f(hw, -hh);

		for (unsigned int i = 0; i < verts.getVertexCount(); i++) verts[i].color = color;

		return verts;
	}

	sf::VertexArray Graphic::makePolygon(const std::vector<sf::Vector2f> &points, const sf::Color &color)
	{
		sf::VertexArray verts(sf::PrimitiveType::TriangleFan, points.size());

		for (std::vector<sf::Vector2f>::size_type i = 0; i < points.size(); i++)
		{
			verts[i].position = points[i];
		}

		for (unsigned int i = 0; i < verts.getVertexCount(); i++) verts[i].color = color;

		return verts;
	}

	sf::VertexArray Graphic::makeLineSegment(float x1, float y1, float x2, float y2, float thickness, const sf::Color &color)
	{
		sf::VertexArray verts(sf::PrimitiveType::TriangleFan, 4);

		sf::Vector2f distLeftNormal(y1 - y2, x2 - x1);
		distLeftNormal *= thickness / VectorMath::length(distLeftNormal);

		verts[0].position = sf::Vector2f(x1 + distLeftNormal.x / 2, y1 + distLeftNormal.y / 2);
		verts[1].position = verts[0].position - distLeftNormal;
		verts[2].position = verts[1].position + sf::Vector2f(x2 - x1, y2 - y1);
		verts[3].position = verts[2].position + distLeftNormal;

		for (unsigned int i = 0; i < verts.getVertexCount(); i++) verts[i].color = color;

		return verts;
	}

	sf::VertexArray Graphic::makeLineSegment(const sf::Vector2f &point1, const sf::Vector2f &point2, float thickness, const sf::Color &color)
	{
		return makeLineSegment(point1.x, point1.y, point2.x, point2.y, thickness, color);
	}
}
