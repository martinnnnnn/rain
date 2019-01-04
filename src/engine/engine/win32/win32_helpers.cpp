#include "win32_helpers.h"


#include "core/core.h"


namespace rain::engine
{
    namespace Win32
    {
        std::string get_exe_path()
        {
            char buffer[MAX_PATH];
            GetModuleFileName(NULL, buffer, MAX_PATH);
            std::string path = std::string(buffer).substr(0, std::string(buffer).find_last_of("\\/"));
            core::String::replace(path, "\\", "/");
            return path;
        }
    }
}