#pragma once



namespace rain::core
{
    template<typename T>
    void set_bit(T& number, T bit)
    {
        number |= 1UL << bit;
    }

    template<typename T>
    void unset_bit(T& number, T bit)
    {
        number &= ~(1UL << bit);
    }
}