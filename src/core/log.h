#pragma once




//#define LOG_WARNING(...) do { rain::Log::log(__FILE__, __LINE__, __VA_ARGS__); } while(false)


namespace rain
{
    //class Log
    //{
    //public:
    //    static void init(bool bindStdIn, bool bindStdOut, bool bindStdErr);

    //    static void log(const char* _str, va_list va)
    //    {
    //        va_list args;
    //        va_start(args, _format);

    //        logVaList(_filePath, _fileLine, _category, _format, args);

    //        va_end(args);
    //    }


    //private:
    //    static constexpr char* INFO{ "[LOG][INFO] " };
    //    static constexpr char* WARNING{ "[LOG][INFO] " };
    //    static constexpr char* ERROR{ "[LOG][INFO] " };


    //    void info(const char* _str, va_list va)
    //    {
    //        static char buffer[512];
    //        //va_list va;
    //        va_start(va, _str);
    //        vsnprintf(buffer, sizeof(buffer), _str, va);
    //        va_end(va);

    //        OutputDebugString(buffer);
    //    }

    //    void logg(std::string str, ...)
    //    {
    //        std::string msg = "[LOG][INFO] " + str;
    //        info(msg.c_str(), __VA_ARGS__);
    //    }
    //};
}

//static char header[]{  };

