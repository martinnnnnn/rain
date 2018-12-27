#pragma once


namespace rain::engine
{
	namespace log_win32
	{
		void log_window(const char* _buffer);
		void log_vs_output(const char* _buffer);
		void log_console(const char* _buffer);

	};
}