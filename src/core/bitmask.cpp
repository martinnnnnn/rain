#include "bitmask.h"



namespace rain
{
    bool GetBit(uint32_t& _mask, const uint32_t bit)
    {
        return ((_mask&(1 << bit)) != 0);
    }

    void TurnOnBit(uint32_t& _mask, const uint32_t bit)
    {
        _mask |= 1 << bit;
    }

    void TurnOnBits(uint32_t& _mask, const uint32_t bits)
    {
        _mask |= bits;
    }

    void ClearBit(uint32_t& _mask, const uint32_t bit)
    {
        _mask &= ~(1 << bit);
    }

    void ToggleBit(uint32_t& _mask, const uint32_t bit)
    {
        _mask ^= 1 << bit;
    }

    // compares 2 bitmasks,
    // if relevantBits is specified, only checks for the relevant bits
    // if not, checks if the bitfields are equal
    bool IsAMatch(uint32_t& _mask1, uint32_t& _mask2, uint32_t _relevantBits)
    {
        if (_relevantBits)
        {
            return (_mask1 & _relevantBits) == (_mask2 & _relevantBits);
        }
        return(_mask1 == _mask2);
    }
}