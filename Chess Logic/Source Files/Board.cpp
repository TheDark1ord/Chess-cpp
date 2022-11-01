#include "..\Header Files\Board.h"

const char* ChessBoard::default_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

ChessBoard::ChessBoard()
	:w_king_pos(0), b_king_pos(0),
	w_queen_pos(0), b_queen_pos(0),
	w_rook_pos(0), b_rook_pos(0),
	w_bishop_pos(0), b_bishop_pos(0),
	w_knight_pos(0), b_knight_pos(0),
	w_pawn_pos(0), b_pawn_pos(0),
	prev_moves(std::stack<Move>()),
	prev_states(std::stack<BoardState>())
{}

ChessBoard::ChessBoard(std::string fen)
{
	load_from_FEN(fen);
}

bool ChessBoard::load_from_FEN(std::string FEN)
{
	// Cleat some variables
	prev_moves = std::stack<Move>();
	prev_states = std::stack<BoardState>();
	pinned_pieces = 0;

	std::regex fen_check("^([pnbrqkPNBRQK1-8]{1,8}\\/?){8}\\s(w|b)\\s(-|K?Q?k?q?)\\s(-|[a-h][3-6])\\s\\d{1,2}\\s\\d+\\s*$");
	if (!std::regex_match(FEN, fen_check)) return false;

	// Split the string by whitespaces
	std::istringstream iss(FEN);
	std::vector<std::string> splitted_string(std::istream_iterator<std::string>{iss},
											 std::istream_iterator<std::string>());
	// Place the pieces
	int x = 0;
	int y = 7;

	for (auto cur_char : splitted_string[0])
	{
		//Empty squares
		if (isdigit(cur_char))
		{
			x += cur_char - '0';
			continue;
		}
		// Line separator
		else if (cur_char == '/')
		{
			x = 0;
			y--;
			continue;
		}
		placePiece(Piece(char_to_piece[cur_char]), y * 8 + (7 - x));
		x++;
	}
	generate_combined_bitboards();

	active_color = splitted_string[1][0] == 'w' ? Color::WHITE : Color::BLACK;

	for (auto current_char : splitted_string[2])
	{
		switch (current_char)
		{
		case('K'):
			castling_rights = castling_rights + CastlingRights::WHITE_SHORT;
			break;
		case('Q'):
			castling_rights = castling_rights + CastlingRights::WHITE_LONG;
			break;
		case('k'):
			castling_rights = castling_rights + CastlingRights::BLACK_SHORT;
			break;
		case('q'):
			castling_rights = castling_rights + CastlingRights::BLACK_LONG;
			break;
		}
	}

	if (splitted_string[3] != "-")
		enPassant_pos = stringPos_to_intPos(splitted_string[3]);
	else enPassant_pos = -1;

	halfmove_clock = atoi(splitted_string[4].c_str());
	fullmove_clock = atoi(splitted_string[5].c_str());

	return true;
}

//TODO:
std::string ChessBoard::export_to_FEN()
{
	return std::string();
}

int ChessBoard::is_occupied(int pos, Color piece_color)
{
	Piece piece = get_piece(pos);

	if (piece == Piece::NONE)
		return 0;
	return ((int)piece % 2) == (piece_color == Color::WHITE ? 1 : -1);
}

Piece ChessBoard::get_piece(int pos)
{
	for (auto i : piece_to_bitboard)
	{
		if (*i.second & (1ull << pos))
		{
			return i.first;
		}
	}
	return Piece::NONE;
}

Color ChessBoard::get_active_color()
{
	return active_color;
}

void ChessBoard::placePiece(Piece piece, int pos)
{
	*piece_to_bitboard[piece] |= 1ull << pos;
}

void ChessBoard::removePiece(Piece piece, int pos)
{ 
	*piece_to_bitboard[piece] &= ~(1ull << pos);
}

void ChessBoard::movePiece(Piece piece, int from, int to)
{ 
	removePiece(piece, from);
	placePiece(piece, to);
	generate_combined_bitboards();
}

void ChessBoard::generate_combined_bitboards()
{
	w_piece_pos = w_king_pos | w_queen_pos | w_rook_pos | w_bishop_pos | w_knight_pos | w_pawn_pos;
	b_piece_pos = b_king_pos | b_queen_pos | b_rook_pos | b_bishop_pos | b_knight_pos | b_pawn_pos;
	all_piece_pos = w_piece_pos | b_piece_pos;
}

BoardState::BoardState()
	:enPassant_pos(-1), halfmove_clock(0), castlingRights(CastlingRights::NONE)
{ }

BoardState::BoardState(const BoardState& other)
	:enPassant_pos(-1), halfmove_clock(0), castlingRights(CastlingRights::NONE)
{ }
