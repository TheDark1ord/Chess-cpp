#pragma once

#include <string>
#include <list>
#include <stack>
#include <unordered_map>
#include <cmath>

#include <istream>
#include <sstream>

#include "Types.h"
#include "BitBoardFunctions.h"

// Variables, that cannot be deduced, undoing the single move
struct BoardState
{
	BoardState();
	BoardState(const BoardState& other);

	int enPassant_pos;
	int halfmove_clock;
	CastlingRights castlingRights;
};

class ChessBoard
{
public:
	ChessBoard();
	ChessBoard(std::string fen);

	bool load_from_FEN(std::string FEN);
	std::string export_to_FEN();

	// 1 - same color, 0 - not occupied, -1 - different color
	int is_occupied(int pos, Color piece_color);
	Piece get_piece(int pos);

	Color get_active_color();

	friend class MoveGenerator;
	struct Move { };

	static const char* default_pos;

	std::unordered_map<char, Piece> char_to_piece = {
		{'k', Piece::B_KING},
		{'q', Piece::B_QUEEN},
		{'r', Piece::B_ROOK},
		{'b', Piece::B_BISHOP},
		{'n', Piece::B_KNIGHT},
		{'p', Piece::B_PAWN},
		{'K', Piece::W_KING},
		{'Q', Piece::W_QUEEN},
		{'R', Piece::W_ROOK},
		{'B', Piece::W_BISHOP},
		{'N', Piece::W_KNIGHT},
		{'P', Piece::W_PAWN},
	};

	std::unordered_map<Piece, BitBoard*> piece_to_bitboard = {
		{ Piece::W_KING, &w_king_pos },
		{ Piece::B_KING, &b_king_pos },

		{ Piece::W_QUEEN, &w_queen_pos },
		{ Piece::B_QUEEN, &b_queen_pos },

		{ Piece::W_ROOK, &w_rook_pos },
		{ Piece::B_ROOK, &b_rook_pos },

		{ Piece::W_BISHOP, &w_bishop_pos },
		{ Piece::B_BISHOP, &b_bishop_pos },

		{ Piece::W_KNIGHT, &w_knight_pos },
		{ Piece::B_KNIGHT, &b_knight_pos },

		{ Piece::W_PAWN, &w_pawn_pos },
		{ Piece::B_PAWN, &b_pawn_pos },
	};

	friend class MoveGenerator;
private:
	inline void placePiece(Piece piece, int pos);
	inline void removePiece(Piece piece, int pos);
	void movePiece(Piece piece, int from, int to);

	std::stack<Move> prev_moves;
	std::stack<BoardState> prev_states;

	// Variables, deduced from FEN string
	Color active_color;
	CastlingRights castling_rights;
	int enPassant_pos;
	int halfmove_clock;
	int fullmove_clock;

	// King checks
	KingCheck whiteCheck = KingCheck::NONE;
	KingCheck blackCheck = KingCheck::NONE;

	BitBoard pinned_pieces;
	// Squares to block the check
	BitBoard to_block;

	// Bitboards
	BitBoard w_king_pos;
	BitBoard b_king_pos;

	BitBoard w_queen_pos;
	BitBoard b_queen_pos;

	BitBoard w_rook_pos;
	BitBoard b_rook_pos;

	BitBoard w_bishop_pos;
	BitBoard b_bishop_pos;

	BitBoard w_knight_pos;
	BitBoard b_knight_pos;

	BitBoard w_pawn_pos;
	BitBoard b_pawn_pos;

	BitBoard w_piece_pos;
	BitBoard b_piece_pos;
	BitBoard all_piece_pos;

	// Generate combined bitboards for white, black and all pieces
	void generate_combined_bitboards();
};