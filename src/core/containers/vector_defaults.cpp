#include "vector_defaults.h"


namespace rain
{
    int* FindInt(Vector<int>* _vector, int toFind)
    {
        auto check = [](int a, int b) { return a == b; };
        return FindItem<int, int>(_vector, toFind, check);
    }

    float* FindFloat(Vector<float>* _vector, float toFind)
    {
        auto check = [](float a, float b) { return a == b; };
        return FindItem<float, float>(_vector, toFind, check);
    }
}