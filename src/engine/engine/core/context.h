#pragma once

#include "core/core.h"

namespace rain::engine
{
    using core::HighResolutionClock;

    struct World;
    struct Config;
    struct Client;

    class data_system;
    class Application;
    class Window;
    class Renderer;
    class Input;

    struct Context : public core::Singleton<Context>
    {
        Context();

        Application* application;
        Config* config;
        Window* window;
        Renderer* renderer;
        Input* input;
        core::Logger* logger;
        data_system* data_sys;
        HighResolutionClock* clock;
    };
}


#define RAIN_CONTEXT                    rain::engine::Context::get()
#define RAIN_CONFIG                     RAIN_CONTEXT->config
#define RAIN_CLOCK                      RAIN_CONTEXT->clock
#define RAIN_RENDERER                   RAIN_CONTEXT->renderer
#define RAIN_WINDOW                     RAIN_CONTEXT->window
#define RAIN_INPUT                      RAIN_CONTEXT->input
#define RAIN_DATA                       RAIN_CONTEXT->data_sys

#define RAIN_FIND_DATA_FROM_PATH(type, path)  RAIN_CONTEXT->data_sys->find_data<type>(path) 
#define RAIN_FIND_DATA_FROM_ID(type, id)      RAIN_CONTEXT->data_sys->find_data<type>(id)

#ifdef _DEBUG
#define RAIN_LOG(...)           do { RAIN_CONTEXT->logger->log_level("[INFO]", __VA_ARGS__); } while(false)
#define RAIN_LOG_DEBUG(...)     do { RAIN_CONTEXT->logger->log_level("[DEBUG]", __VA_ARGS__); } while(false)
#define RAIN_LOG_NETWORK(...)   do { RAIN_CONTEXT->logger->log_level("[NETWORK]", __VA_ARGS__); } while(false)
#define RAIN_LOG_PROFILE(...)   do { RAIN_CONTEXT->logger->log_level("[PROFILE]", __VA_ARGS__); } while(false)
#define RAIN_LOG_WARNING(...)   do { RAIN_CONTEXT->logger->log_max(__FILE__, __LINE__, "[WARNING]", __VA_ARGS__); } while(false)
#define RAIN_LOG_ERROR(...)     do { RAIN_CONTEXT->logger->log_max(__FILE__, __LINE__, "[ERROR]", __VA_ARGS__); } while(false)
#define RAIN_LOG_RAW(...)       do { RAIN_CONTEXT->logger->log_raw(__VA_ARGS__); } while(false)
#else
#define RAIN_LOG(...)
#define RAIN_LOG_DEBUG(...)
#define RAIN_LOG_NETWORK(...)
#define RAIN_LOG_PROFILE(...)
#define RAIN_LOG_WARNING(...)
#define RAIN_LOG_ERROR(...)
#define RAIN_LOG_RAW(...)
#endif