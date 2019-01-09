#pragma once


namespace rain::core::math
{
    struct Sphere
    {
        Sphere()
            : Sphere(0.0f, glm::vec3(0.0f, 0.0f, 0.0f))
        {}

        Sphere(float _radius, glm::vec3 _offset)
            : radius(_radius)
            , offset(_offset)
        {}

        // offset from the center of the object - which must have a transform
        f32 radius;
        glm::vec3 offset;
    };
}