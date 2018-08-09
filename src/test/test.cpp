#include "test.h"


namespace rain
{
    void TestEQ(Test* _test, bool _condition, const char* _messageFail, const char* _messageSuccess)
    {
        _test->testsCount++;
        if (_condition)
        {
            std::cout << _messageSuccess << std::endl;
        }
        else
        {
            _test->testsFailed++;
            std::cout << _messageFail << std::endl;
        }
    }
}