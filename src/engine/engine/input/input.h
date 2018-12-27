//#pragma once
//
//#include <array>
//#include <stdio.h>
//
//#include "core/types.h"
//
//using namespace rain::core;
//
//namespace rain::engine
//{
//	template<typename T>
//	struct Input
//	{
//		Input();
//
//		int		init();
//		void	shutdown();
//		void	update();
//
//		void	set_cursor_pos(u32 _x, u32 _y);
//		void	update_absolute_mouse_pos(u32 _x, u32 _y);
//		bool	is_key_pressed(u32 _code) const;
//		bool	is_key_released(u32 _code)  const;
//
//		i32 x_offset;
//		i32 y_offset;
//		u32 x_screen_pos;
//		u32 y_screen_pos;
//		bool mouse_lock;
//
//	protected:
//
//		u32 x_center;
//		u32 y_center;
//
//		static constexpr u32 KEYS_COUNT = 256;
//		std::array<BYTE, KEYS_COUNT> m_keysA;
//		std::array<BYTE, KEYS_COUNT> m_keysB;
//		std::array<BYTE, KEYS_COUNT>* m_keysCurrent;
//	};
//
//	template<typename T>
//	rain::engine::Input<T>::Input() : mouse_lock(true)
//		, m_keysCurrent(&m_keysA)
//	{
//		memset(m_keysA.data(), 0, m_keysA.size());
//		memset(m_keysB.data(), 0, m_keysA.size());
//	}
//
//	template<typename T>
//	int rain::engine::Input<T>::init()
//	{
//		static_cast<T*>(this)->init();
//	}
//
//	template<typename T>
//	void rain::engine::Input<T>::shutdown()
//	{
//		static_cast<T*>(this)->shutdown();
//	}
//
//	template<typename T>
//	void rain::engine::Input<T>::update()
//	{
//		static_cast<T*>(this)->update();
//	}
//
//	template<typename T>
//	void rain::engine::Input<T>::set_cursor_pos(u32 _x, u32 _y)
//	{
//		static_cast<T*>(this)->set_cursor_pos(_x, _y);
//	}
//
//	template<typename T>
//	bool rain::engine::Input<T>::is_key_pressed(u32 _code) const
//	{
//		return ((*m_keysCurrent)[_code] & 0x80);
//	}
//
//	template<typename T>
//	bool rain::engine::Input<T>::is_key_released(u32 _code) const
//	{
//		if (!is_key_pressed(_code))
//		{
//			if (m_keysCurrent == &m_keysA)
//			{
//				return (m_keysB[_code] & 0x80);
//			}
//			else
//			{
//				return (m_keysA[_code] & 0x80);
//			}
//		}
//		return false;
//	}
//
//	template<typename T>
//	void rain::engine::Input<T>::update_absolute_mouse_pos(u32 _x, u32 _y)
//	{
//		x_screen_pos = _x;
//		y_screen_pos = _y;
//	}
//
//}

#pragma once

#include <vector>
#include <array>
#include <algorithm>

#include "input_event.h"
#include "engine/win32/keycodes_win32.h"

namespace rain::engine
{
	class Input
	{
	public:
		Input();

		int init();
		void shutdown();

		void update();

		void set_cursor_pos(u32 _x, u32 _y);
		void update_absolute_mouse_pos(u32 _x, u32 _y);

		bool is_key_pressed(u32 _code) const;
		bool is_key_released(u32 _code)  const;

		i32 x_offset;
		i32 y_offset;
		u32 x_screen_pos;
		u32 y_screen_pos;
		bool mouse_lock;
	private:
		static constexpr u32 KEYS_COUNT = 256;

		std::vector<InputEvent> m_inputEvents;
		LPDIRECTINPUT8  m_diObject;
		LPDIRECTINPUTDEVICE8 m_keyboard;
		LPDIRECTINPUTDEVICE8 m_mouse;
		std::array<BYTE, KEYS_COUNT> m_keysA;
		std::array<BYTE, KEYS_COUNT> m_keysB;
		std::array<BYTE, KEYS_COUNT>* m_keysCurrent;
		u32 x_center;
		u32 y_center;
	};

}

