#include "context.h"

#include "engine/win32/win32_helpers.h"
#include "engine/core/logger.h"
#include "engine/core/config.h"
#include "engine/core/id_generator.h"
#include "engine/data/data_system.h"
#include "engine/win32/win32_window.h"
#include "engine/win32/win32_input.h"
#include "engine/gfx/ogl/ogl_renderer.h"


namespace rain::engine
{

    Context::Context()
        : config(nullptr)
        , window(nullptr)
        , renderer(nullptr)
        , input(nullptr)
        , logger(nullptr)
        , id_generator(nullptr)
        , data_system(nullptr)
        , clock(nullptr) {}
}