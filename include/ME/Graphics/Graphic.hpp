#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "../IComponent.hpp"

namespace me
{
	/// Custom implementation of sf::Shape to allow more flexibility + animation.
	/// A lot of this is basically copy-pasted from sf::Shape.
	class Graphic : public IComponent
	{
	protected:
		std::shared_ptr<sf::Texture> m_texture;		//texture to fill the shape with, can be null
		sf::IntRect			m_textureRect;	//area of texture to draw
		sf::VertexArray		m_vertices;		//vertices to draw
		sf::FloatRect		m_bounds;		//bounding box containing the vertices

	public:
		/// Draw the graphic in the specified target
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		/// Set every vertex to the specified color
		void fillWithColor(const sf::Color& color);

		void setTexture(std::shared_ptr<sf::Texture> texture, bool resetRect = true);
		void setTextureRect(const sf::IntRect &rect);
		void setVertices(const sf::VertexArray &verts);

		// Inherited Component methods
		virtual std::string getType() const;
		virtual IComponent * clone() const;


		Graphic();
		/// Graphic without a texture
		Graphic(const sf::VertexArray &vertices);
		/// Completely custom graphic
		Graphic(std::shared_ptr<sf::Texture> texture, const sf::IntRect &textureRect, const sf::VertexArray &vertices);
		/// Copy constructor
		Graphic(const Graphic &copy);
		virtual ~Graphic();

		/// Create a circle with the origin at the center.
		static Graphic* makeCircle(float radius, unsigned int pointCount, const sf::Color &color = sf::Color::White);
		/// Create a rectangle with the specified height and width.
		static Graphic* makeRect(float width, float height, const sf::Color &color = sf::Color::White);
		/// Create a polygon with the given points.
		static Graphic* makePolygon(const std::vector<sf::Vector2f> &points, const sf::Color &color = sf::Color::White);

	protected:
		/// This should be called every time a point's position changes
		void updateVertices();

	private:
		/// Update texture coordinates to keep the texture from deforming
		void updateTexCoords();
	};
}



#endif //GRAPHIC_HPP
