#include "test.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

namespace rain
{
    void TestIntro(Test* _test, const char* _name)
    {
        _test->name = (char*)malloc(sizeof(char) * (strlen(_name + 1)));
        strcpy_s(_test->name, strlen(_name) + 1, _name);
        _test->testsCount = 0;
        _test->testsFailed = 0;

        printf("\n-----------------------------\n%s\n-----------------------------\n", _test->name);
    }

    void TestEQ(Test* _test, bool _condition, const char* _name)
    {
        printf("\nTesting %s : ", _name);
        _test->testsCount++;
        if (_condition)
        {
            printf("Test passed\n");
        }
        else
        {
            _test->testsFailed++;
            printf("Test failed\n");
        }
    }

    void TestRecap(Test* _test)
    {
        printf("\n-----------------------------\nTEST REPORT : %s\n-----------------------------\n", _test->name);
        printf("Number of tests : %i\n", _test->testsCount);
        printf("Number of tests succeded : %i\n", _test->testsCount - _test->testsFailed);
        printf("Number of tests failed : %i\n", _test->testsFailed);
        printf("-----------------------------\n");
    }
}