#pragma once
#include "Types.h"

struct MagicNumber
{
	// Masks only relevant squares for piece lookup
	BitBoard mask;
	// Magic factor
	BitBoard magic;
};

// Magic bitboard
//BitBoard bishopAttacks[64][512];
//BitBoard rookAttacks[64][4096];

