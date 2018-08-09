#pragma once

#include "core/types.h"


namespace rain
{
    struct Test
    {
        char* name;
        u32 testsCount;
        u32 testsFailed;
    };

    void TestIntro(Test* _test, const char* _name);
    void TestEQ(Test* _test, bool _condition, const char* _name);
    void TestRecap(Test* _test);
}