#pragma once


#include "core/types.h"

namespace rain
{
    void AssertDebugBreak(bool _condition, const char* _fileName, u32 _lineNumber, const char* _conditionText, const char* _msg);
    
}

#ifdef _DEBUG
#define RAIN_ASSERT_EQ(_condition, msg) AssertDebugBreak(_condition, __FILE__, __LINE__, #_condition, msg)
#else
#define RAIN_ASSERT_EQ(_condition, msg)
#endif