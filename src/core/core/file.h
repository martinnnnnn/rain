#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "core/core.h"

namespace rain::core
{
    namespace File
    {
		bool	write(const std::string& path, u8* content, u32 size);
		i32		read(const std::string& path, u8** content);
		void	read(const std::string& path, std::string& content);
		bool	exists(const std::string& name);
    }
}