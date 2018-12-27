#pragma once

#include <vector>
#include <string>

#include "core/core.h"

namespace rain::core
{
	template<typename T>
	struct DataHandle
	{
		DataHandle(const std::string& _path)
			: path(_path)
			, id(get_name_id(path.get_path_relative().c_str()))
		{
			data = new T();
			data->load(path.get_path_absolute());
		}

		DataHandle()
			: path("")
			, id()
			, data(nullptr) {}

		FilePath path;
		unique_id id;
		T* data;

		static unique_id (*get_name_id) (const char*);
	};

	template <typename T>
	struct DataHandleContainer
	{
		void load_data(const std::string& _path)
		{
			DataHandle<T>* handle = new DataHandle<T>(_path);
			datas.push_back(handle);
		}

		std::vector<DataHandle<T>*> datas;
	};

}
