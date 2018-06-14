#pragma once



#include <stdint.h>

namespace rain
{
	//struct Bitmask
	//{
	//	uint64_t bits;
	//};

    // TODO(martin): refactor to make it clearer that getbit requires a shift value whereas getbits takes in a bitmask that will be tested directly, without any bitshit.
    // TODO(martin): same for turnonbit / turnonbits
    bool GetBit(uint64_t& _mask, const uint64_t bit);
    bool GetBits(uint64_t& _mask, const uint64_t bits);
    void TurnOnBit(uint64_t& _mask, const uint64_t bit);
    void TurnOnBits(uint64_t& _mask, const uint64_t bits);
    void ClearBit(uint64_t& _mask, const uint64_t bit);
    void ToggleBit(uint64_t& _mask, const uint64_t bit);

    // compares 2 bitmasks,
	// if relevantBits is specified, only checks for the relevant bits
	// if not, checks if the bitfields are equal
    bool IsAMatch(uint64_t& _mask1, uint64_t& _mask2, uint64_t _relevantBits = 0);
}