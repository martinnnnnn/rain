#pragma once

#include "core/core.h"
#include "engine/win32/win32_helpers.h"


namespace rain::engine
{
    using core::HighResolutionClock;

    struct IdGenerator;
    struct World;
    struct Config;
    struct DataSystem;
    struct Client;

    class Application;
    class Window;
    class Renderer;
    class Input;
    class Logger;

    struct Context : public core::Singleton<Context>
    {
        Context();

        Config* config;
        Window* window;
        Renderer* renderer;
        Input* input;
        Logger* logger;
        IdGenerator* id_generator;
        DataSystem* data_system;
        HighResolutionClock* clock;
    };
}


#define RAIN_CONTEXT                    rain::engine::Context::get()
#define RAIN_CONFIG                     RAIN_CONTEXT->config
#define RAIN_CLOCK                      RAIN_CONTEXT->clock
#define RAIN_RENDERER                   RAIN_CONTEXT->renderer
#define RAIN_WINDOW                     RAIN_CONTEXT->window
#define RAIN_INPUT                      RAIN_CONTEXT->input
#define RAIN_DATA                       RAIN_CONTEXT->data_system

#define RAIN_NEW_ID                     RAIN_CONTEXT->id_generator->get_new_unique_id()
#define RAIN_NEW_NAME_ID(name)          RAIN_CONTEXT->id_generator->get_new_unique_id(name)
#define RAIN_FIND_DATA_FROM_PATH(path)  RAIN_CONTEXT->data_system->find_mesh(path) 
#define RAIN_FIND_DATA_FROM_ID(id)      RAIN_CONTEXT->data_system->find_mesh(id)

#ifdef _DEBUG
#define RAIN_LOG(...)           do { RAIN_CONTEXT->logger->log_level(Logger::LOG_INFO, __VA_ARGS__); } while(false)
#define RAIN_LOG_DEBUG(...)     do { RAIN_CONTEXT->logger->log_level(Logger::LOG_DEBUG, __VA_ARGS__); } while(false)
#define RAIN_LOG_NETWORK(...)   do { RAIN_CONTEXT->logger->log_level(Logger::LOG_NETWORK, __VA_ARGS__); } while(false)
#define RAIN_LOG_PROFILE(...)   do { RAIN_CONTEXT->logger->log_level(Logger::LOG_PROFILE, __VA_ARGS__); } while(false)
#define RAIN_LOG_WARNING(...)   do { RAIN_CONTEXT->logger->log_max(__FILE__, __LINE__, Logger::LOG_WARNING, __VA_ARGS__); } while(false)
#define RAIN_LOG_ERROR(...)     do { RAIN_CONTEXT->logger->log_max(__FILE__, __LINE__, Logger::LOG_ERROR, __VA_ARGS__); } while(false)
#define RAIN_LOG_RAW(...)       do { RAIN_CONTEXT->logger->log_raw(__VA_ARGS__); } while(false)
#else
#define RAIN_LOG(...)
#define RAIN_LOG_DEBUG(...)
#define RAIN_LOG_WARNING(...)
#define RAIN_LOG_ERROR(...)
#endif