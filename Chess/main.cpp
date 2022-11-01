#include "pch.h"

#include "BoardRenderer.h"

#define HIDECONSOLE 1
#if HIDECONSOLE
#pragma comment( linker, "/subsystem:windows" )
#else
#pragma comment( linker, "/subsystem:console" )
#endif

int frameRate = 0;
int selected_square = -1;

void HandleEvent(sf::Event& ev, sf::RenderWindow* window);
// Load svg file and load it into the target texture

ChessBoard board(ChessBoard::default_pos);
MoveGenerator generator(&board);

sf::VertexArray square_pattern;
std::vector<sf::Texture> piece_textures;
std::vector<sf::Sprite> sprites;

sf::Texture ornament_texture;
sf::RectangleShape ornament_shape;

namespace r = boardRender;
r::FlippedRenderer* renderer;

int main()
{
	sf::ContextSettings settings;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(r::Width, r::Height), "Chess", sf::Style::Default, settings);
	renderer = new r::FlippedRenderer(window, true, true); 

	if (frameRate != 0)
		window->setFramerateLimit(frameRate);

	sf::Transform transform;
	transform.translate(r::x_offset, r::y_offset);
	sf::RenderStates offset(transform);

	r::loadResources(&piece_textures, &sprites, r::piece_filenames, r::square_size);
	r::generateBoardVertices(&square_pattern, { r::x_offset, r::y_offset }, r::square_size, r::light_yellow, r::green);

	ornament_texture.loadFromFile("Resources\\Ornament.png");
	ornament_shape.setTexture(&ornament_texture);
	ornament_shape.setSize(sf::Vector2f(r::square_size, r::square_size));

	generator.generate_pawn_moves();

	while (window->isOpen())
	{
		sf::Event ev;
		while (window->pollEvent(ev))
			HandleEvent(ev, window);

		window->clear();

		renderer->draw(square_pattern, offset);
		r::drawPieces(board, r::square_size, sprites, renderer, offset);
		r::drawPieceMoves(generator, &ornament_shape, selected_square, r::square_size, renderer, offset);

		window->display();
	}
}

void HandleEvent(sf::Event& ev, sf::RenderWindow* window)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		window->close();
		break;

	case sf::Event::MouseButtonReleased:
		if (ev.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f flipped_point = renderer->flip_point(sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y ));
			int new_selected_square = vectorPos_to_intPos((flipped_point.x - r::x_offset) / r::square_size, (flipped_point.y - r::y_offset) / r::square_size);

			if (selected_square != -1)
				r::highlightSquare(&square_pattern, selected_square, (selected_square % 8 + selected_square / 8) % 2 ? r::green : r::light_yellow);
			if (new_selected_square != selected_square)
			{
				// Try to make a move
				if (generator.make_a_move(selected_square, new_selected_square))
				{
					selected_square = -1;
					return;
				}

				r::highlightSquare(&square_pattern, new_selected_square, r::regular_yellow);
				selected_square = new_selected_square;
			} else
				selected_square = -1;
			
		}
		break;
	case sf::Event::KeyPressed:
		switch (ev.key.code)
		{
		case sf::Keyboard::F:
			// Flip the switches
			renderer->flip_x ^= 1;
			renderer->flip_y ^= 1;
			break;
		}
		break;
	}
	
}