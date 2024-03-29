#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <SFML/Graphics.hpp>
#include <memory>

namespace me
{
	/// Custom implementation of sf::Shape to allow more flexibility + animation.
	/// A lot of this is basically copy-pasted from sf::Shape.
	class Graphic
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



		Graphic();
		/// Graphic without a texture
		Graphic(const sf::VertexArray &vertices);
		/// Completely custom graphic
		Graphic(std::shared_ptr<sf::Texture> texture, const sf::IntRect &textureRect, const sf::VertexArray &vertices);
		/// Copy constructor
		Graphic(const Graphic &copy);
		/// Move constructor
		Graphic(Graphic&& move);
		virtual ~Graphic();

		/// Move assignment operator
		Graphic& operator=(Graphic&& other);



		/// Create a circle with the origin at the center.
		static sf::VertexArray makeCircle(float radius, unsigned int pointCount, const sf::Color &color = sf::Color::White);
		/// Create a rectangle with the specified height and width.
		static sf::VertexArray makeRect(float width, float height, const sf::Color &color = sf::Color::White);
		/// Create a polygon with the given points.
		static sf::VertexArray makePolygon(const std::vector<sf::Vector2f> &points, const sf::Color &color = sf::Color::White);
		/// Create a line segment between two given points.
		static sf::VertexArray makeLineSegment(float x1, float y1, float x2, float y2, float thickness, const sf::Color &color = sf::Color::White);
		static sf::VertexArray makeLineSegment(const sf::Vector2f &point1, const sf::Vector2f &point2, float thickness, const sf::Color &color = sf::Color::White);

	protected:
		/// This should be called every time a point's position changes
		void updateVertices();

	private:
		/// Update texture coordinates to keep the texture from deforming
		void updateTexCoords();
	};
}



#endif //GRAPHIC_HPP
