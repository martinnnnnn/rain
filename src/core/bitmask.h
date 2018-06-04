#pragma once



#include <stdint.h>



using Bitset = uint32_t;



namespace rain
{
	struct Bitmask
	{
		uint32_t bits;
	};

	bool GetBit(Bitmask _mask, const uint32_t bit)
	{
		return ((_mask.bits&(1 << bit)) != 0);
	}

	void TurnOnBit(Bitmask _mask, const uint32_t bit)
	{
		_mask.bits |= 1 << bit;
	}

	void TurnOnBits(Bitmask _mask, const uint32_t bits)
	{
		_mask.bits |= bits;
	}

	void ClearBit(Bitmask _mask, const uint32_t bit)
	{
		_mask.bits &= ~(1 << bit);
	}

	void ToggleBit(Bitmask _mask, const uint32_t bit)
	{
		_mask.bits ^= 1 << bit;
	}

	// compares 2 bitmasks,
	// if relevantBits is specified, only checks for the relevant bits
	// if not, checks if the bitfields are equal
	bool IsAMatch(Bitmask _mask1, Bitmask _mask2, uint32_t _relevantBits = 0)
	{
		if (_relevantBits)
		{
			return (_mask1.bits & _relevantBits) == (_mask2.bits & _relevantBits);
		}
		return(_mask1.bits == _mask2.bits);
	}
}