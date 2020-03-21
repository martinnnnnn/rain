#include "config.h"

//#include "engine/win32/win32_application.h"
#include "engine/core/json_reader.h"
#include "core/core.h"

namespace rain::engine
{
    void Config::init()
    {
		data_root = RAIN_DATA_ROOT;
		runtime_data_root = RAIN_RUNTIME_DATA_ROOT;
		std::string config_path = data_root + RAIN_CONFIG_FILE_NAME;

		assert(core::file::exists(data_root + RAIN_CONFIG_FILE_NAME) && "Data root path is wrong : you will not be able to continue further.");

        file.open(data_root + RAIN_CONFIG_FILE_NAME);
        json_reader::read_config(file.read(), *this);
    }
}