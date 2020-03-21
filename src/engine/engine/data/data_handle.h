#pragma once


#include "core/core.h"


namespace rain::engine
{
    template<typename T>
    struct handle
    {
        core::file_path path;
        core::uuid id;
        T* data;
    };
}