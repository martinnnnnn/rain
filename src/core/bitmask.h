#pragma once



#include <stdint.h>



using Bitset = uint32_t;



namespace rain
{
	//struct Bitmask
	//{
	//	uint32_t bits;
	//};

    bool GetBit(uint32_t& _mask, const uint32_t bit);
    void TurnOnBit(uint32_t& _mask, const uint32_t bit);
    void TurnOnBits(uint32_t& _mask, const uint32_t bits);
    void ClearBit(uint32_t& _mask, const uint32_t bit);
    void ToggleBit(uint32_t& _mask, const uint32_t bit);

    // compares 2 bitmasks,
	// if relevantBits is specified, only checks for the relevant bits
	// if not, checks if the bitfields are equal
    bool IsAMatch(uint32_t& _mask1, uint32_t& _mask2, uint32_t _relevantBits = 0);
}