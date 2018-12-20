#pragma once

#include <cassert>

#include "core/types.h"

namespace rain
{
    struct Texture
    {
        u32 id;
        i32 format;
        unsigned char* data;
        int width;
        int height;

        void load(const std::string& _path)
        {
            assert(false);
        }
    };
}