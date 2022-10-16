#include "..\Header Files\BitBoardFunctions.h"

std::vector<int> get_set_bits(BitBoard board)
{
	std::vector<int> set_bits;
	set_bits.reserve(8);

	while (board != 0)
	{
		// Get the rightmost set bit
		set_bits.push_back((int)std::log2(board & (~board + 1)));
		// Unset this bit
		board &= board - 1;
	}

	return set_bits;
}


// Code to check if the provided position is valid
#if POS_CHECK

#define CHECK_STRING_POS(string_pos) if(!std::regex_match(string_pos, std::regex("[a-h][1-8]"))) throw std::runtime_error("Invalid string position");
#define CHECK_INT_POS(int_pos) if (!(int_pos >= 0 && int_pos <= 64)) throw std::runtime_error("Invalid board position");

#else
// Skip the check
#define CHECK_STRING_POS(string_pos)
#define CHECK_INT_POS(int_pos)

#endif

int stringPos_to_intPos(std::string pos)
{
	CHECK_STRING_POS(pos)
	return (pos.at(0) - 'a') * 8 + atoi(pos.substr(1, 1).c_str());
}

std::string intPos_to_stringPos(int pos)
{
	CHECK_INT_POS(pos)
	return std::string(std::string(1, 'a' + (pos / 8)) + std::to_string(pos % 8));
}

int vectorPos_to_intPos(int x, int y)
{
	return y * 8 + x;
}
