#include "file.h"


namespace rain::core
{
	bool File::write(const std::string& path, u8* content, u32 size)
	{
		FILE* file;
		if (fopen_s(&file, path.c_str(), "wb") == 0)
		{
			fwrite(content, sizeof(u8), size, file);
			fclose(file);
			return true;
		}
		return false;
	}

	i32 File::read(const std::string& path, u8** content)
	{
		FILE *f;
		if (fopen_s(&f, path.c_str(), "rb") == 0)
		{
			fseek(f, 0, SEEK_END);
			u32 fsize = ftell(f);
			fseek(f, 0, SEEK_SET);

			*content = (u8*)malloc(fsize + 1);
			fread(*content, 1, fsize, f);
			fclose(f);

			(*content)[fsize] = 0;
			return fsize;
		}
		return -1;
	}

	void File::read(const std::string& path, std::string& content)
	{
		FILE *f;
		if (fopen_s(&f, path.c_str(), "rb") == 0)
		{
			fseek(f, 0, SEEK_END);
			u32 fsize = ftell(f);
			fseek(f, 0, SEEK_SET);

			content.resize(fsize, '\0');
			fread(&content[0], sizeof(char), (size_t)fsize, f);
			fclose(f);
		}
	}

	bool File::exists(const std::string& name)
	{
		FILE* file = NULL;
		if (fopen_s(&file, name.c_str(), "r") == 0)
		{
			fclose(file);
			return true;
		}
		else
		{
			return false;
		}
	}
}


