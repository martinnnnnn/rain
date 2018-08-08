#pragma once



namespace rain
{
    namespace math
    {
        template<typename T>
        void Clamp(T& _val, T _min, T _max)
        {
            _val = (_val < _min) ? _min : (_val > _max) ? _max : _val;
        }
    }
}