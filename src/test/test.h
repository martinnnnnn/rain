#pragma once

#include "core/types.h"

#include <iostream>

namespace rain
{
    struct Test
    {
        u32 testsCount;
        u32 testsFailed;
    };

    void TestEQ(Test* _test, bool _condition, const char* _messageFail, const char* _messageSuccess);
}