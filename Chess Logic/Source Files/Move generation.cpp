#include "..\Header Files\Move Generation.h"
#include "..\Header Files\Board.h"


Move::Move(MoveType type, int from, int to)
	:move_type(type), from(from), to(to),
	captured_piece(Piece::NONE), promoted_to(Piece::NONE)
{ }

Move::Move(MoveType type, int from, int to, Piece captured, Piece promoted)
	: move_type(type), from(from), to(to),
	captured_piece(captured), promoted_to(promoted)
{ }

MoveGenerator::MoveGenerator(ChessBoard* board)
	:board(board),
	w_king_attacks(0), b_king_attacks(0),
	w_queen_attacks(0), b_queen_attacks(0),
	w_rook_attacks(0), b_rook_attacks(0),
	w_bishop_attacks(0), b_bishop_attacks(0),
	w_knight_attacks(0), b_knight_attacks(0),
	w_pawn_attacks(0), b_pawn_attacks(0),
	w_piece_attacks(0), b_piece_attacks(0)
{ }

void MoveGenerator::generate_pawn_moves()
{
	// Edge pawm captures should be handled separetly
	static constexpr BitBoard left_edge = 0x8080808080808080;
	static constexpr BitBoard right_edge = 0x0101010101010101;
	// To Handle promotions
	static constexpr BitBoard top_edge = 0xff00000000000000;
	static constexpr BitBoard bottom_edge = 0x00000000000000ff;

	// For double moves
	static constexpr BitBoard row_2 = 0x000000000000ff00;
	static constexpr BitBoard row_7 = 0x00ff000000000000;

	static constexpr BitBoard edges = left_edge | right_edge;

	// Clear the bitboards
	w_pawn_attacks = 0;
	b_pawn_attacks = 0;

	BitBoard single_pawn_moves;
	BitBoard double_pawn_moves;

	BitBoard left_pawn_attacks;
	BitBoard right_pawn_attacks;

	BitBoard left_pawn_captures;
	BitBoard right_pawn_captures;

	/// Generate moves for white pawns
	single_pawn_moves = (board->w_pawn_pos << 8) & ~(board->all_piece_pos);
	// Convert bitboard to move array
	for (auto move : get_set_bits(single_pawn_moves & ~top_edge))
		generated_moves.push_back(Move(MoveType::NORMAL, move - 8, move));
	for (auto move : get_set_bits(single_pawn_moves & top_edge))
		generated_moves.push_back(Move(MoveType::PROMOTION, move - 8, move));

	double_pawn_moves = ((board->w_pawn_pos & row_2) << 16) & ~(board->all_piece_pos);
	// Convert bitboard to move array
	for (auto move : get_set_bits(double_pawn_moves))
		generated_moves.push_back(Move(MoveType::DOUBLE_MOVE, move - 16, move));

	// Generate pawn captures for all pawns except edge pawns
	left_pawn_attacks = ((board->w_pawn_pos & ~left_edge) << 9);
	right_pawn_attacks = ((board->w_pawn_pos & ~right_edge) << 7);
	w_pawn_attacks |= left_pawn_attacks;
	w_pawn_attacks |= right_pawn_attacks;

	left_pawn_captures = left_pawn_attacks & (board->b_piece_pos | (1ull << board->enPassant_pos) * (board->enPassant_pos != -1));
	right_pawn_captures = right_pawn_attacks & (board->b_piece_pos | (1ull << board->enPassant_pos) * (board->enPassant_pos != -1));

	// Convert bitboard to move array
	for (auto move : get_set_bits(left_pawn_captures & ~top_edge))
		generated_moves.push_back(Move(MoveType::CAPTURE, move - 9, move));
	for (auto move : get_set_bits(right_pawn_captures & ~top_edge))
		generated_moves.push_back(Move(MoveType::CAPTURE, move - 7, move));

	for (auto move : get_set_bits(left_pawn_captures & top_edge))
		generated_moves.push_back(Move(MoveType::PROMOTION_CAPTURE, move - 9, move));
	for (auto move : get_set_bits(right_pawn_captures & top_edge))
		generated_moves.push_back(Move(MoveType::PROMOTION_CAPTURE, move - 7, move));


	/// Almost the same, but for black pawns

	single_pawn_moves = (board->b_pawn_pos >> 8) & ~(board->all_piece_pos);
	// Convert bitboard to move array
	for (auto move : get_set_bits(single_pawn_moves & ~bottom_edge))
		generated_moves.push_back(Move(MoveType::NORMAL, move + 8, move));
	for (auto move : get_set_bits(single_pawn_moves& bottom_edge))
		generated_moves.push_back(Move(MoveType::PROMOTION, move + 8, move));

	double_pawn_moves = ((board->b_pawn_pos & row_7) >> 16) & ~(board->all_piece_pos);
	// Convert bitboard to move array
	for (auto move : get_set_bits(double_pawn_moves))
		generated_moves.push_back(Move(MoveType::DOUBLE_MOVE, move + 16, move));

	// Generate pawn captures for all pawns except edge pawns
	left_pawn_attacks = ((board->b_pawn_pos & ~left_edge) >> 9);
	right_pawn_attacks = ((board->b_pawn_pos & ~right_edge) >> 7);
	b_pawn_attacks |= left_pawn_attacks;
	b_pawn_attacks |= right_pawn_attacks;

	left_pawn_captures = left_pawn_attacks & (board->w_piece_pos | (1ull << board->enPassant_pos) * (board->enPassant_pos != -1));
	right_pawn_captures = right_pawn_attacks & (board->w_piece_pos | (1ull << board->enPassant_pos) * (board->enPassant_pos != -1));

	// Convert bitboard to move array
	for (auto move : get_set_bits(left_pawn_captures & ~bottom_edge))
		generated_moves.push_back(Move(MoveType::CAPTURE, move + 9, move));
	for (auto move : get_set_bits(right_pawn_captures & ~bottom_edge))
		generated_moves.push_back(Move(MoveType::CAPTURE, move + 7, move));

	for (auto move : get_set_bits(left_pawn_captures& bottom_edge))
		generated_moves.push_back(Move(MoveType::PROMOTION_CAPTURE, move + 9, move));
	for (auto move : get_set_bits(right_pawn_captures& bottom_edge))
		generated_moves.push_back(Move(MoveType::PROMOTION_CAPTURE, move + 7, move));
}

std::vector<Move> MoveGenerator::get_moves(int pos) const
{
	std::vector<Move> piece_moves;
	std::copy_if(generated_moves.begin(), generated_moves.end(), std::back_inserter(piece_moves),
				 [&pos] (Move move) { return move.from == pos; });
	
	return piece_moves;
}

bool MoveGenerator::make_a_move(int from, int to)
{
	auto piece_moves = get_moves(from);
	MoveType move_type;
	if (!std::any_of(piece_moves.begin(), piece_moves.end(), [&to, &move_type] (Move m) { move_type = m.move_type; return m.to == to; }))
		return false;

	switch (move_type)
	{
	case MoveType::CAPTURE:
		if (to == board->enPassant_pos)
		{
			if (board->active_color == Color::WHITE)
				board->removePiece(board->get_piece(to - 8), to - 8);
			else
				board->removePiece(board->get_piece(to + 8), to + 8);
			break;
		}
		board->removePiece(board->get_piece(to), to);
		board->generate_combined_bitboards();
		break;

	case MoveType::PROMOTION_CAPTURE:
		board->removePiece(board->get_piece(to), to);

		// Fallthrough
	case MoveType::PROMOTION:
		Piece promoted_piece = board->get_piece(from);
		// Automatically place the queen
		board->placePiece(
			static_cast<Piece>(static_cast<int>(Piece::W_QUEEN) +
			static_cast<int>(static_cast<int>(promoted_piece) % 2)),  // Change the color of a placed piece
			to 
		);
		board->removePiece(promoted_piece, from);
		board->generate_combined_bitboards();

		// Skip moving the piece
		goto ChangeVariables;
	}
	board->movePiece(board->get_piece(from), from, to);

ChangeVariables:
	// Flip the color
	board->active_color = static_cast<Color>(static_cast<int>(board->active_color) ^ 1);

	// TODO: add color
	board->enPassant_pos = move_type == MoveType::DOUBLE_MOVE ? to - 8: -1;

	generated_moves.clear();
	generate_pawn_moves();
	return true;
}
