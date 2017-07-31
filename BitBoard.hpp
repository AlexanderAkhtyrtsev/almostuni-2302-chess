/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef BITBOARD__
#define BITBOARD__

#include <cstdint>
#include <vector>
#include <bitset>
#include <utility>

class BitBoard
{
	constexpr static size_t BITBLOCKSIZE=32;
	
	uint8_t h, w;
	std::vector<std::bitset<BITBLOCKSIZE>> data;
public:
	BitBoard() = delete;
	BitBoard(const BitBoard& other) = default;
	BitBoard(uint8_t height, uint8_t width);
	uint8_t getHeight() const;
	uint8_t getWidth() const;
	void set(uint8_t f, uint8_t r, bool val);
	bool get(uint8_t f, uint8_t r) const;
	int countBits() const;
	std::pair<uint8_t, uint8_t> getFirstOccurance() const; // file, rank
	BitBoard operator&(const BitBoard& that) const;
	BitBoard operator&=(const BitBoard& that);
	BitBoard operator|(const BitBoard& that) const;
	BitBoard operator|=(const BitBoard& that);
};

#endif