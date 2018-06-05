#pragma once


namespace rain
{
    struct Component
    {
        enum class Type
        {
            NONE = (1 << 0),
            TRANSFORM = (1 << 1),
            MODEL = (1 << 2),
            LIGHT = (1 << 3),
            MAX_COUNT = (1 << 31)
        };

        Type type;

        Component(Type _type) : type(_type) {}
        virtual ~Component() {}
    };
}