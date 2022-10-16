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

	static constexpr BitBoard edges = left_edge | right_edge;

	// Clear the bitboards
	w_pawn_attacks = 0;
	b_pawn_attacks = 0;


	/// Generate moves for white pawns
	BitBoard single_pawn_moves = (board->w_pawn_pos << 8) & ~(board->all_piece_pos);
	// Convert bitboard to move array
	for (auto move : get_set_bits(single_pawn_moves & ~top_edge))
		generated_moves.push_back(Move(MoveType::NORMAL, move - 8, move));
	for (auto move : get_set_bits(single_pawn_moves & top_edge))
		generated_moves.push_back(Move(MoveType::PROMOTION, move - 8, move));

	BitBoard double_pawn_moves = (single_pawn_moves << 8) & ~(board->all_piece_pos);
	// Convert bitboard to move array
	for (auto move : get_set_bits(double_pawn_moves))
		generated_moves.push_back(Move(MoveType::NORMAL, move - 16, move));

	// Generate pawn captures for all pawns except edge pawns
	BitBoard left_pawn_captures = ((single_pawn_moves & ~left_edge) << 9) & board->b_piece_pos;
	BitBoard right_pawn_captures = ((single_pawn_moves & ~right_edge) << 7) & board->b_piece_pos;
	w_pawn_attacks |= left_pawn_captures;
	w_pawn_attacks |= right_pawn_captures;


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

	double_pawn_moves = (single_pawn_moves >> 8) & ~(board->all_piece_pos);
	// Convert bitboard to move array
	for (auto move : get_set_bits(double_pawn_moves))
		generated_moves.push_back(Move(MoveType::NORMAL, move + 16, move));

	// Generate pawn captures for all pawns except edge pawns
	left_pawn_captures = ((single_pawn_moves & ~left_edge) >> 9) & board->w_piece_pos;
	right_pawn_captures = ((single_pawn_moves & ~right_edge) >> 7) & board->w_piece_pos;
	b_pawn_attacks |= left_pawn_captures;
	b_pawn_attacks |= right_pawn_captures;


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
