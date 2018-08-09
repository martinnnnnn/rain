#include "test/vector_test.h"

#include "core/containers/vector.h"
#include <iostream>
#include "test.h"

namespace rain
{
    

    void TestVectorCreate()
    {


        std::cout << std::endl;
        std::cout << "------------------------------"
            << std::endl << "----- TEST VECTOR CREATE -----"
            << std::endl << "------------------------------";

        Vector<int>* v1 = CreateVector<int>(5, 2);

        std::cout << std::endl;
        std::cout << "Size (0 ?) :" << v1->size << std::endl;
        std::cout << "Capacity (5 ?) :" << v1->capacity << std::endl;
        std::cout << "Step (2 ?) :" << v1->resizeStep << std::endl;
        std::cout << "Expected result : []" << std::endl;
        std::cout << "Result : [";
        for (u32 i = 0; i < v1->size; ++i)
        {
            std::cout << i << ",";
        }
        std::cout << "]" << std::endl;

        Test test;
        TestIntro(&test, "TEST VECTOR CREATE");
        TestEQ(&test, v1->size == 0, "CreateVector<int>() size");
        TestEQ(&test, v1->capacity == 5, "CreateVector<int>() capacity");
        TestEQ(&test, v1->resizeStep == 2, "CreateVector<int>() step");
        TestRecap(&test);
    }

    void TestVectorInit()
    {
        std::cout << std::endl;
        std::cout <<        "----------------------------"
            << std::endl << "----- TEST VECTOR INIT -----"
            << std::endl << "----------------------------";

        Vector<int> v1;
        InitVector<int>(&v1, 5, 2);

        std::cout << std::endl;
        std::cout << "Size (0 ?) :" << v1.size << std::endl;
        std::cout << "Capacity (5 ?) :" << v1.capacity << std::endl;
        std::cout << "Step (2 ?) :" << v1.resizeStep << std::endl;
        std::cout << "Expected result : []" << std::endl;
        std::cout << "Result : [";
        for (u32 i = 0; i < v1.size; ++i)
        {
            std::cout << i << ",";
        }
        std::cout << "]" << std::endl;
    }

    void TestVectorAdd()
    {
        std::cout << std::endl;
        std::cout <<        "---------------------------"
            << std::endl << "----- TEST VECTOR ADD -----"
            << std::endl << "---------------------------";

        Vector<int> v1;
        InitVector<int>(&v1, 5, 2);
        AddItem<int>(&v1, 3);
        AddItem<int>(&v1, 2);
        AddItem<int>(&v1, 1);

        std::cout << std::endl;
        std::cout << "Size (3 ?) :" << v1.size << std::endl;
        std::cout << "Capacity (5 ?) :" << v1.capacity << std::endl;
        std::cout << "Step (2 ?) :" << v1.resizeStep << std::endl;
 
        std::cout << "Expected result : [3,2,1,]" << std::endl;
        std::cout << "Result : [";
        for (u32 i = 0; i < v1.size; ++i)
        {
            std::cout << v1.items[i] << ",";
        }
        std::cout << "]" << std::endl;

        AddItem<int>(&v1, 6);
        AddItem<int>(&v1, 7);
        AddItem<int>(&v1, 8);
        std::cout << std::endl;
        std::cout << "Size (6 ?) :" << v1.size << std::endl;
        std::cout << "Capacity (7 ?) :" << v1.capacity << std::endl;
        std::cout << "Step (2 ?) :" << v1.resizeStep << std::endl;
        std::cout << "Expected result : [3,2,1,6,7,8,]" << std::endl;
        std::cout << "Result : [";
        for (u32 i = 0; i < v1.size; ++i)
        {
            std::cout << v1.items[i] << ",";
        }
        std::cout << "]" << std::endl;
    }

    void TestVectorAddRange()
    {
        std::cout << std::endl;
        std::cout <<        "---------------------------------"
            << std::endl << "----- TEST VECTOR ADD RANGE -----"
            << std::endl << "---------------------------------";

        Vector<int> v1;
        InitVector<int>(&v1, 5, 2);
        AddItem<int>(&v1, 1);
        AddItem<int>(&v1, 2);
        AddItem<int>(&v1, 3);
        int range[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        AddRangeItems<int>(&v1, range, 10);

        std::cout << std::endl;
        std::cout << "Size (13 ?) :" << v1.size << std::endl;
        std::cout << "Capacity (13 ?) :" << v1.capacity << std::endl;
        std::cout << "Step (2 ?) :" << v1.resizeStep << std::endl;

        std::cout << "Expected result : [1,2,3,0,1,2,3,4,5,6,7,8,9,]" << std::endl;
        std::cout << "Result : [";
        for (u32 i = 0; i < v1.size; ++i)
        {
            std::cout << v1.items[i] << ",";
        }
        std::cout << "]" << std::endl;

        Vector<int> v2;
        InitVector<int>(&v2, 5, 2);
        AddRangeItems<int>(&v2, range, 10);

        std::cout << std::endl;
        std::cout << "Size (10 ?) :" << v2.size << std::endl;
        std::cout << "Capacity (11 ?) :" << v2.capacity << std::endl;
        std::cout << "Step (2 ?) :" << v2.resizeStep << std::endl;

        std::cout << "Expected result : [0,1,2,3,4,5,6,7,8,9,]" << std::endl;
        std::cout << "Result : [";
        for (u32 i = 0; i < v2.size; ++i)
        {
            std::cout << v2.items[i] << ",";
        }
        std::cout << "]" << std::endl;
    }

    void TestVectorFind()
    {
        std::cout << std::endl;
        std::cout <<        "----------------------------"
            << std::endl << "----- TEST VECTOR FIND -----"
            << std::endl << "----------------------------";

        Vector<int> v1;
        InitVector<int>(&v1, 5, 2);
        AddItem<int>(&v1, 1);
        AddItem<int>(&v1, 2);
        AddItem<int>(&v1, 3);

        std::cout << std::endl;
        std::cout << "Expected result : 2" << std::endl;
        std::cout << "Result : ";
        auto checkIntEq = [](int a, int b) { return a == b; };
        int* foundItem = FindItem<int, int>(&v1, 2, checkIntEq);
        std::cout << *foundItem << std::endl;

        std::cout << std::endl;
        std::cout << "Expected result : Item not found" << std::endl;
        std::cout << "Result : ";
        foundItem = FindItem<int, int>(&v1, 6, checkIntEq);
        if (!foundItem) std::cout << "Item not found" << std::endl;
        else std::cout << "ERROR :: Item found" << std::endl;
    }

    void TestVectorRemove()
    {
        std::cout << std::endl;
        std::cout <<        "------------------------------"
            << std::endl << "----- TEST VECTOR REMOVE -----"
            << std::endl << "------------------------------";

        Vector<int> v1;
        InitVector<int>(&v1, 5, 2);
        AddItem<int>(&v1, 1);
        AddItem<int>(&v1, 2);
        AddItem<int>(&v1, 3);

        std::cout << std::endl;
        std::cout << "Expected result : [1,3,]" << std::endl;
        std::cout << "Result : ";
        auto checkIntEq = [](int a, int b) { return a == b; };
        RemoveItem<int>(&v1, FindItem<int, int>(&v1, 2, checkIntEq));
        for (u32 i = 0; i < v1.size; ++i)
        {
            std::cout << v1.items[i] << ",";
        }
        std::cout << "]" << std::endl;

        AddItem<int>(&v1, 6);
        AddItem<int>(&v1, 7);
        AddItem<int>(&v1, 8);
        AddItem<int>(&v1, 9);

        std::cout << std::endl;
        std::cout << "Expected result : [1,9,6,7,8,]" << std::endl;
        std::cout << "Result : [";
        RemoveItem<int>(&v1, FindItem<int, int>(&v1, 3, checkIntEq));
        for (u32 i = 0; i < v1.size; ++i)
        {
            std::cout << v1.items[i] << ",";
        }
        std::cout << "]" << std::endl;
    }

    void RunTestVector()
    {
        TestVectorCreate();
        TestVectorInit();
        TestVectorAdd();
        TestVectorAddRange();
        TestVectorFind();
        TestVectorRemove();
    }
}