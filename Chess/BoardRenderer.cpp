#include "pch.h"
#include "BoardRenderer.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"


namespace boardRender
{
	int Width = 1280;
	int Height = 1280;

	int square_size = std::min(Width / 8, Height / 8);
	float x_offset = std::max(float(Width - Height) / 2, 0.0f);
	float y_offset = std::max(float(Height - Width) / 2, 0.0f);

	extern sf::Color green(0x769656ff);
	extern sf::Color light_yellow(0xeeeed2ff);
	extern sf::Color regular_yellow(0xbaca44ff);

	const char* piece_filenames[12]
	{
		"Resources\\ChessPieces\\wK.svg", "Resources\\ChessPieces\\bK.svg",
		"Resources\\ChessPieces\\wQ.svg", "Resources\\ChessPieces\\bQ.svg",
		"Resources\\ChessPieces\\wR.svg", "Resources\\ChessPieces\\bR.svg",
		"Resources\\ChessPieces\\wN.svg", "Resources\\ChessPieces\\bN.svg",
		"Resources\\ChessPieces\\wB.svg", "Resources\\ChessPieces\\bB.svg",
		"Resources\\ChessPieces\\wP.svg", "Resources\\ChessPieces\\bP.svg",
	};


	FlippedRenderer::FlippedRenderer(sf::RenderTarget* render_target, bool flip_x, bool flip_y)
		:render_target(render_target), flip_x(flip_x), flip_y(flip_y)
	{ }

	FlippedRenderer::~FlippedRenderer()
	{ }

	void FlippedRenderer::clear(const sf::Color& color)
	{
		render_target->clear(color);
	}

	void FlippedRenderer::setView(const sf::View& view)
	{
		render_target->setView(view);
	}

	sf::Vector2u FlippedRenderer::getSize() const
	{
		return render_target->getSize();
	}

	bool FlippedRenderer::setActive(bool active)
	{
		return render_target->setActive(active);
	}

	void FlippedRenderer::draw_p(const sf::CircleShape & drawable, const sf::RenderStates render_states)
	{
		sf::CircleShape clone(drawable);
		sf::Rect object_bounds = drawable.getGlobalBounds();

		float flipped_x = !flip_x ? object_bounds.left : Width - object_bounds.left - object_bounds.width;
		float flipped_y = !flip_y ? object_bounds.top : Height - object_bounds.top - object_bounds.height;
		clone.setPosition(flipped_x, flipped_y);

		render_target->draw(clone, render_states);
	}

	void FlippedRenderer::draw_p(const sf::RectangleShape& drawable, const sf::RenderStates render_states)
	{
		sf::RectangleShape clone(drawable);
		sf::Rect object_bounds = drawable.getGlobalBounds();

		float flipped_x = !flip_x ? object_bounds.left : Width - object_bounds.left - object_bounds.width;
		float flipped_y = !flip_y ? object_bounds.top : Height - object_bounds.top - object_bounds.height;
		clone.setPosition(flipped_x, flipped_y);

		render_target->draw(clone, render_states);
	}

	void FlippedRenderer::draw_p(const sf::ConvexShape& drawable, const sf::RenderStates render_states)
	{
		sf::ConvexShape clone(drawable);
		sf::Rect object_bounds = drawable.getGlobalBounds();

		float flipped_x = !flip_x ? object_bounds.left : Width - object_bounds.left - object_bounds.width;
		float flipped_y = !flip_y ? object_bounds.top : Height - object_bounds.top - object_bounds.height;
		clone.setPosition(flipped_x, flipped_y);

		render_target->draw(clone, render_states);
	}


	void FlippedRenderer::draw_p(const sf::Sprite& drawable, const sf::RenderStates render_states)
	{ 
		sf::Sprite clone(drawable);
		sf::Rect object_bounds = drawable.getGlobalBounds();

		float flipped_x = !flip_x ? object_bounds.left : Width - object_bounds.left - object_bounds.width;
		float flipped_y = !flip_y ? object_bounds.top : Height - object_bounds.top - object_bounds.height;
		clone.setPosition(flipped_x, flipped_y);

		render_target->draw(clone, render_states);
	}

	void FlippedRenderer::draw_p(const sf::Text& drawable, const sf::RenderStates render_states)
	{ 
		sf::Text clone(drawable);
		sf::Rect object_bounds = drawable.getGlobalBounds();

		float flipped_x = !flip_x ? object_bounds.left : Width - object_bounds.left - object_bounds.width;
		float flipped_y = !flip_y ? object_bounds.top : Height - object_bounds.top - object_bounds.height;
		clone.setPosition(flipped_x, flipped_y);

		render_target->draw(clone, render_states);
	}

	void FlippedRenderer::draw_p(const sf::VertexArray& verticies, const sf::RenderStates render_states)
	{
		sf::VertexArray translated_array(verticies.getPrimitiveType(), verticies.getVertexCount());
		sf::Vertex translated_vertex;

		for (int i = 0; i < verticies.getVertexCount(); i++)
		{
			translated_vertex.position.x =
				!flip_x ?
				verticies[i].position.x :
				Width - verticies[i].position.x;

			translated_vertex.position.y =
				!flip_y ?
				verticies[i].position.y :
				Width - verticies[i].position.y;

			translated_vertex.color = verticies[i].color;
			translated_array[i] = translated_vertex;
		}

		render_target->draw(translated_array, render_states);
	}

	void FlippedRenderer::draw_p(std::shared_ptr<sf::Shape> shape_ptr, const sf::RenderStates render_states)
	{
		sf::Rect object_bounds = shape_ptr.get()->getGlobalBounds();

		float flipped_x = !flip_x ? object_bounds.left : Width - object_bounds.left + object_bounds.width;
		float flipped_y = !flip_y ? object_bounds.top : Height - object_bounds.top + object_bounds.height;
		shape_ptr.get()->setPosition(flipped_x, flipped_y);

		render_target->draw(*shape_ptr.get(), render_states);
	}

	sf::Vector2f FlippedRenderer::flip_point(sf::Vector2f point)
	{
		point.x = flip_x ? Width - point.x : point.x;
		point.y = flip_y ? Height - point.y : point.y;

		return point;
	}

	void FlippedRenderer::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
	{
		if (typeid(drawable) == typeid(sf::CircleShape))
			draw_p((dynamic_cast<const sf::CircleShape&>(drawable)), states);

		else if (typeid(drawable) == typeid(sf::RectangleShape))
			draw_p((dynamic_cast<const sf::RectangleShape&>(drawable)), states);

		else if (typeid(drawable) == typeid(sf::ConvexShape))
			draw_p((dynamic_cast<const sf::ConvexShape&>(drawable)), states);

		else if (typeid(drawable) == typeid(sf::Sprite))
			draw_p((dynamic_cast<const sf::Sprite&>(drawable)), states);

		else if (typeid(drawable) == typeid(sf::Text))
			draw_p((dynamic_cast<const sf::Text&>(drawable)), states);

		else if (typeid(drawable) == typeid(sf::VertexArray))
			draw_p((dynamic_cast<const sf::VertexArray&>(drawable)), states);
	}

	bool loadResources(std::vector<sf::Texture>* texture_out, std::vector<sf::Sprite>* sprite_out, const char* filename_arr[12], int square_size, float dpi, const std::string_view& units)
	{
		// Load texture from svg file
		auto load_texture_from_file = [] (sf::Texture& target_texture, const char* filename, float dpi, const std::string& units) {
			std::unique_ptr< NSVGimage, void(*)(NSVGimage*)> svg_image(nsvgParseFromFile(filename, units.c_str(), dpi), &nsvgDelete);
			std::unique_ptr<NSVGrasterizer, void(*)(NSVGrasterizer*)> rast(nsvgCreateRasterizer(), &nsvgDeleteRasterizer);

			if (svg_image == NULL)
			{
				printf("Could not open svg image\n");
				return false;
			}
			if (rast == NULL)
			{
				printf("Could not create rasterizer\n");
				return false;
			}

			int image_width = (int)svg_image->width;
			int image_height = (int)svg_image->height;
			std::unique_ptr<unsigned char[]> image(new unsigned char[(uint64_t)image_width * image_height * 4]);

			if (image == NULL)
			{
				printf("Could not allocate image buffer\n");
				return false;
			}

			nsvgRasterize(rast.get(), svg_image.get(), 0, 0, 1, image.get(), image_width, image_height, image_width * 4);

			target_texture.create(image_width, image_height);
			target_texture.update(image.get(), image_width, image_height, 0, 0);

			return true;
		};
		

		texture_out->resize(12);
		sprite_out->resize(12);

		for (int i = 0; i < 12; i++)
		{
			if (!load_texture_from_file((*texture_out)[i], filename_arr[i],
				dpi, static_cast<std::string>(units)))
			{
				return false;
			}
			(*sprite_out)[i].setTexture((*texture_out)[i]);
		}

		// Set the texture scale, so that a sprite fits into a square
		float texture_scale = (float)square_size / (float)(*texture_out)[0].getSize().x;
		for (int i = 0; i < 12; i++)
			(*sprite_out)[i].setScale(sf::Vector2f(texture_scale, texture_scale));

		return true;
	}

	void generateBoardVertices(sf::VertexArray* vertex_out, const sf::Vector2f& offset, int square_size, sf::Color light_color, sf::Color dark_color)
	{
		*vertex_out = sf::VertexArray(sf::Quads, 256);
		// Fill the array
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				(*vertex_out)[(y * 8 + x) * 4 + 0].position = sf::Vector2f((x + 0) * square_size, (y + 1) * square_size);
				(*vertex_out)[(y * 8 + x) * 4 + 1].position = sf::Vector2f((x + 0) * square_size, (y + 0) * square_size);
				(*vertex_out)[(y * 8 + x) * 4 + 2].position = sf::Vector2f((x + 1) * square_size, (y + 0) * square_size);
				(*vertex_out)[(y * 8 + x) * 4 + 3].position = sf::Vector2f((x + 1) * square_size, (y + 1) * square_size);

				(*vertex_out)[(y * 8 + x) * 4 + 0].color = (x + y) % 2 == 0 ? light_color : dark_color;
				(*vertex_out)[(y * 8 + x) * 4 + 1].color = (x + y) % 2 == 0 ? light_color : dark_color;
				(*vertex_out)[(y * 8 + x) * 4 + 2].color = (x + y) % 2 == 0 ? light_color : dark_color;
				(*vertex_out)[(y * 8 + x) * 4 + 3].color = (x + y) % 2 == 0 ? light_color : dark_color;
			}
		}
	}

	void highlightSquare(sf::VertexArray* vertex_out, int square_number, sf::Color highlight_color)
	{
		(*vertex_out)[(uint64_t)square_number * 4 + 0].color = highlight_color;
		(*vertex_out)[(uint64_t)square_number * 4 + 1].color = highlight_color;
		(*vertex_out)[(uint64_t)square_number * 4 + 2].color = highlight_color;
		(*vertex_out)[(uint64_t)square_number * 4 + 3].color = highlight_color;
	}

	void drawPieces(ChessBoard& chess_board, int square_size, std::vector<sf::Sprite>& sprites, FlippedRenderer* render_target, const sf::RenderStates& render_states)
	{
		for (int int_piece = static_cast<int>(Piece::W_KING); int_piece <= static_cast<int>(Piece::B_PAWN); int_piece++)
		{
			BitBoard current_board = *chess_board.piece_to_bitboard[static_cast<Piece>(int_piece)];

			std::vector<int> set_bits = get_set_bits(current_board);
			for (int i : set_bits)
			{
				sprites[(uint64_t)int_piece - 1].setPosition(sf::Vector2f((i % 8) * square_size, (i / 8) * square_size));
				render_target->draw(sprites[(uint64_t)int_piece - 1], render_states);
			}
		}
	}

	void drawPieceMoves(const MoveGenerator& move_generator, sf::Shape* move_indicator, int selected_square, int square_size, FlippedRenderer* render_target, const sf::RenderStates& render_states)
	{
		if (move_indicator != nullptr)
		{
			for (const Move& move : move_generator.get_moves(selected_square))
			{
				move_indicator->setPosition(sf::Vector2f((float(move.to % 8)) * square_size, (float(move.to / 8) ) * square_size));
				render_target->draw(*move_indicator, render_states);
			}
		return;      
		}

		static sf::CircleShape default_indicator;
		if (default_indicator.getRadius() != (0.55f * square_size / 2))
		{
			default_indicator.setRadius(0.55f * square_size / 2);
			default_indicator.setOutlineThickness(0.05f * square_size);
			default_indicator.setFillColor(sf::Color::Transparent);
			default_indicator.setOutlineColor(sf::Color(0x000000C8));
		}

		for (const Move& move : move_generator.get_moves(selected_square))
		{
			default_indicator.setPosition(sf::Vector2f((float(move.to % 8) + 0.5f) * square_size - default_indicator.getRadius(),
										  ((float(move.to / 8) + 0.5f) * square_size - default_indicator.getRadius())));
			render_target->draw(default_indicator, render_states);
		}
	}
}