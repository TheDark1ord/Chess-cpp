#pragma once

#include <unordered_map>

#include "Board.h"
#include "MagicNumbers.h"

struct Move
{
	Move(MoveType type, int from, int to);
	Move(MoveType type, int from, int to, Piece captured, Piece promoted);

	MoveType move_type;
	int from;
	int to;

	Piece captured_piece;
	Piece promoted_to;
};

class MoveGenerator
{
public:
	MoveGenerator(ChessBoard* board);

	void generate_pawn_moves();

	// Get all the moves, avalible for a piece at that position
	std::vector<Move> get_moves(int pos) const;

private:
	ChessBoard* board;
	std::vector<Move> generated_moves;

	BitBoard w_king_attacks;
	BitBoard b_king_attacks;

	BitBoard w_queen_attacks;
	BitBoard b_queen_attacks;

	BitBoard w_rook_attacks;
	BitBoard b_rook_attacks;

	BitBoard w_bishop_attacks;
	BitBoard b_bishop_attacks;
	
	BitBoard w_knight_attacks;
	BitBoard b_knight_attacks;
	
	BitBoard w_pawn_attacks;
	BitBoard b_pawn_attacks;

	// Combined bitboard of all attacked squares
	BitBoard w_piece_attacks;
	BitBoard b_piece_attacks;
};