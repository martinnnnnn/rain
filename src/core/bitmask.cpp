#include "bitmask.h"



namespace rain
{
    bool GetBit(uint64_t& _mask, const uint64_t bit)
    {
        return ((_mask&(1 << bit)) != 0);
    }

    bool GetBits(uint64_t& _mask, const uint64_t bits)
    {
        return ((_mask& bits) == bits);
    }

    void TurnOnBit(uint64_t& _mask, const uint64_t bit)
    {
        _mask |= 1 << bit;
    }

    void TurnOnBits(uint64_t& _mask, const uint64_t bits)
    {
        _mask |= bits;
    }

    void ClearBit(uint64_t& _mask, const uint64_t bit)
    {
        _mask &= ~(1 << bit);
    }

    void ToggleBit(uint64_t& _mask, const uint64_t bit)
    {
        _mask ^= 1 << bit;
    }

    bool IsAMatch(uint64_t& _mask1, uint64_t& _mask2, uint64_t _relevantBits)
    {
        if (_relevantBits)
        {
            return (_mask1 & _relevantBits) == (_mask2 & _relevantBits);
        }
        return(_mask1 == _mask2);
    }
}