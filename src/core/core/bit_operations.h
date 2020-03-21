#pragma once



namespace rain::core
{
    template<typename T>
    void bit_set(T& bitfield, T position, T value)
    {
        unsigned long newbit = !!value;
        bitfield ^= (-newbit ^ bitfield) & (1UL << position);
    }

    template<typename T>
    void bit_set(T& bitfield, T position)
    {
        bitfield |= 1UL << position;
    }

    template<typename T>
    void bit_unset(T& bitfield, T position)
    {
        bitfield &= ~(1UL << position);
    }

    template<typename T>
    void bit_toggle(T& bitfield, T position)
    {
        bitfield ^= 1UL << position;
    }

    template<typename T>
    T bit_check(T& bitfield, T position)
    {
        return (bitfield >> position) & 1U;
    }



}