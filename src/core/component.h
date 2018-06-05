#pragma once


namespace rain
{
    struct Component
    {
        enum class Type
        {
            TRANSFORM = (1 << 0),
            MODEL = (1 << 1),
            LIGHT = (1 << 2),
            SHADER = (1 << 3),
            MATERIAL = (1 << 4),

            MAX_COUNT = (1 << 31)
        };

        Type type;

        Component(Type _type) : type(_type) {}
        virtual ~Component() {}

    };
}