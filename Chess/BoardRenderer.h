#pragma once

#include "pch.h"

namespace boardRender
{
	extern int Width, Height;

	/// Some variables for drawing on screen
	extern const char* piece_filenames[12];
	extern int square_size;
	extern float x_offset;
	extern float y_offset;
	
	 // Color Palete
	extern sf::Color green;
	extern sf::Color light_yellow;
	extern sf::Color regular_yellow;

	// A class that inverts x and(or) y axis before rendering an object(does not flip the textures)
	class FlippedRenderer : public sf::RenderTarget
	{
	public:
		FlippedRenderer(sf::RenderTarget* render_target, bool flip_x = false, bool flip_y = false);
		~FlippedRenderer();

		void clear(const sf::Color& color);
		void setView(const sf::View& view);
		sf::Vector2u getSize() const;
		bool setActive(bool active = true);

		void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
		void draw(const sf::Vertex* verticies, std::size_t vertex_count, sf::PrimitiveType primitive_type, const sf::RenderStates render_states = sf::RenderStates::Default);
		void draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states = sf::RenderStates::Default);
		void draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states = sf::RenderStates::Default);

		// Flip the given point according to flip_x and flip_y
		sf::Vector2f flip_point(sf::Vector2f point);

		bool flip_x, flip_y;
	private:
		void draw_p(const sf::CircleShape& drawable, const sf::RenderStates render_states = sf::RenderStates::Default);
		void draw_p(const sf::RectangleShape& drawable, const sf::RenderStates render_states = sf::RenderStates::Default);
		void draw_p(const sf::ConvexShape& drawable, const sf::RenderStates render_states = sf::RenderStates::Default);
		void draw_p(const sf::Sprite& drawable, const sf::RenderStates render_states = sf::RenderStates::Default);
		void draw_p(const sf::Text& drawable, const sf::RenderStates render_states = sf::RenderStates::Default);
		void draw_p(const sf::VertexArray& verticies, const sf::RenderStates render_states = sf::RenderStates::Default);
		void draw_p(const sf::VertexBuffer& verticies, const sf::RenderStates render_states = sf::RenderStates::Default);

		sf::RenderTarget* render_target;

		// A general function for drawing shapes
		void draw_p(std::shared_ptr<sf::Shape> shape_ptr, const sf::RenderStates render_states = sf::RenderStates::Default);
	};

	// Load svg files from filename_arr into texture vector and associate them with sprites from sprite array
	// Then set the scale of sprites to match the square size
	bool loadResources(std::vector<sf::Texture>* texture_out, std::vector<sf::Sprite>* sprite_out,
					   const char* filename_arr[12], int square_size,
					   float dpi = 50, const std::string_view& units = "px");

	// Generate vertex array for displaying the board
	void generateBoardVertices(sf::VertexArray* vertex_out, const sf::Vector2f& offset, int square_size,
							   sf::Color light_color, sf::Color dark_color);
	void highlightSquare(sf::VertexArray* vertex_out, int square_number, sf::Color highlight_color);

	// Board offset should be passed in board states
	void drawPieces(ChessBoard& chess_board, int square_size, std::vector<sf::Sprite>& sprites,
					FlippedRenderer* render_target, const sf::RenderStates& render_states);
	// Provide nullptr in move indicator field to set the default indicator
	void drawPieceMoves(const MoveGenerator& move_generator, sf::Shape* move_indicator, int selected_square, int square_size, FlippedRenderer* render_target, const sf::RenderStates& render_states);
}