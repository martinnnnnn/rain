#include "assert/rain_assert.h"


#include <iostream>

#include "utility/incl_3d.h"

namespace rain
{
    void AssertDebugBreak(bool _condition, const char* _fileName, u32 _lineNumber, const char* _conditionText, const char* _msg)
    {
        if (_condition)
            return;

        std::cout << ">> ERROR RAISED at " << glfwGetTime() << " in file " << _fileName << " at line " << _lineNumber << " by " << _conditionText << "." << std::endl
            << ">> ERROR MESSAGE : " << _msg << std::endl << std::endl;

        __debugbreak();
    }
}