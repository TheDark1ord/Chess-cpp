#pragma once

#include <cstddef>

// Type, reperesenting bitset of 64 bits, one bit for each cell
typedef uint64_t BitBoard;

enum struct Color
{
	WHITE, BLACK
};

enum struct MoveType
{
	NONE,

	NORMAL,
	DOUBLE_MOVE,
	CAPTURE,
	CASTLING,
	EN_PASSANT,
	PROMOTION,
	PROMOTION_CAPTURE
};

enum struct CastlingRights : int
{
	NONE = 0,
	WHITE_SHORT = 1 << 0,
	WHITE_LONG = 1 << 1,
	BLACK_SHORT = 1 << 2,
	BLACK_LONG = 1 << 3,

	SHORT_CASTLING = WHITE_SHORT | BLACK_SHORT,
	LONG_CASTLING = WHITE_LONG | BLACK_LONG,
	WHITE_CASTLING = WHITE_SHORT | WHITE_LONG,
	BLACK_CASTLING = BLACK_SHORT | BLACK_LONG,

	ANY_CASTLING = WHITE_CASTLING | BLACK_CASTLING
};

inline CastlingRights operator+(CastlingRights lhv, CastlingRights rhv)
{
	return static_cast<CastlingRights>(static_cast<int>(lhv) + static_cast<int>(rhv));
}

enum struct PieceType
{
	NONE = 0,

	PAWN = 1,
	KNIGHT = 3,
	BISHOP = 5,
	ROOK = 7,
	QUEEN = 9,
	KING = 11
};

enum struct Piece : int
{
	NONE = -1,

	W_KING, B_KING,
	W_QUEEN, B_QUEEN,
	W_ROOK, B_ROOK,
	W_KNIGHT, B_KNIGHT,
	W_BISHOP, B_BISHOP,
	W_PAWN, B_PAWN,
};

enum struct DirectionOffsets : int
{
	WEST = -1,
	EAST = 1,
	NORTH = -8,
	SOUTH = 8,

	NORTH_WEST = NORTH + WEST,
	NORTH_EAST = NORTH + EAST,
	SOUTH_WEST = SOUTH + WEST,
	SOUTH_EAST = SOUTH + EAST,
};

enum struct KingCheck
{
	NONE, SINGLE, DOUBLE,
};