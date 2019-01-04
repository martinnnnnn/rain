#pragma once

#include "core/core.h"
#include "engine/win32/win32_helpers.h"

#define RAIN_CONFIG_FILE_NAME "/config.rain"

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

#define RAIN_CONFIG                     rain::engine::Context::get()->config
#define RAIN_NEW_ID                     rain::engine::Context::get()->id_generator->get_new_unique_id()
#define RAIN_NEW_NAME_ID(name)          rain::engine::Context::get()->id_generator->get_new_unique_id(name)
#define RAIN_FIND_DATA_FROM_PATH(path)  rain::engine::Context::get()->data_system->find_mesh(path) 
#define RAIN_FIND_DATA_FROM_ID(id)      rain::engine::Context::get()->data_system->find_mesh(id)
#define RAIN_RENDERER                   rain::engine::Context::get()->renderer
#define RAIN_APPLICATION                rain::engine::Context::get()
#define RAIN_INPUT                      rain::engine::Context::get()->input
#define RAIN_WINDOW                     rain::engine::Context::get()->window
#define RAIN_WORLD                      rain::engine::Context::get()->world
#define RAIN_CLOCK                      rain::engine::Context::get()->clock

#ifdef _DEBUG
#define RAIN_LOG(...)           do { rain::engine::Context::get()->logger->log_level(Logger::LOG_INFO, __VA_ARGS__); } while(false)
#define RAIN_LOG_DEBUG(...)     do { rain::engine::Context::get()->logger->log_level(Logger::LOG_DEBUG, __VA_ARGS__); } while(false)
#define RAIN_LOG_NETWORK(...)   do { rain::engine::Context::get()->logger->log_level(Logger::LOG_NETWORK, __VA_ARGS__); } while(false)
#define RAIN_LOG_PROFILE(...)   do { rain::engine::Context::get()->logger->log_level(Logger::LOG_PROFILE, __VA_ARGS__); } while(false)
#define RAIN_LOG_WARNING(...)   do { rain::engine::Context::get()->logger->log_max(__FILE__, __LINE__, Logger::LOG_WARNING, __VA_ARGS__); } while(false)
#define RAIN_LOG_ERROR(...)     do { rain::engine::Context::get()->logger->log_max(__FILE__, __LINE__, Logger::LOG_ERROR, __VA_ARGS__); } while(false)
#define RAIN_LOG_RAW(...)       do { rain::engine::Context::get()->logger->log_raw(__VA_ARGS__); } while(false)
#else
#define RAIN_LOG(...)
#define RAIN_LOG_DEBUG(...)
#define RAIN_LOG_WARNING(...)
#define RAIN_LOG_ERROR(...)
#endif