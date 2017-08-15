#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <SFML/Graphics.hpp>

namespace me
{
	/// Custom implementation of sf::Shape to allow more flexibility + animation
	class Graphic : public sf::Drawable, public sf::Transformable
	{
	protected:
		const sf::Texture*	m_texture;		//texture to fill the shape with, can be null
		sf::IntRect			m_textureRect;	//area of texture to draw
		sf::VertexArray		m_vertices;		//vertices to draw
		sf::FloatRect		m_bounds;		//bounding box containing the vertices

	public:
		/// This should be called every frame
		virtual void update(sf::Time timeElapsed) = 0;

		Graphic();
		virtual ~Graphic();

	protected:
		/// This should be called every time a point's position changes
		void updateVertices();

		/// Set every vertex to the specified color
		void fillWithColor(const sf::Color& color);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		/// Update texture coordinates to keep the texture from deforming
		void updateTexCoords();
	};
}



#endif //GRAPHIC_HPP