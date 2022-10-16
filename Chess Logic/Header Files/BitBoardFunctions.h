#pragma once

#include <vector>
#include <string>
#include <regex>

#include "Types.h"

// Get the indexes of all set bits in a bitboard
// Return value: array of indexes, number of set bits
std::vector<int> get_set_bits(BitBoard board);

// determines if the functions below should check their input
// before returning the value
#define POS_CHECK 1
// Convert chess pos notation (b2, a4, e4, etc.) to square number
int stringPos_to_intPos(std::string pos);
// Convert square number to chess pos notation (b2, a4, e4, etc.)
std::string intPos_to_stringPos(int pos);

int vectorPos_to_intPos(int x, int y);