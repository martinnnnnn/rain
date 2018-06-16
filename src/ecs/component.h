//#pragma once
//
//#include <stdint.h>
//
//#define INIT_COMPONENT_TYPE(type) 
//
//#define BIT(x) (static_cast<uint64_t>(1) << x)
//namespace rain
//{
//    struct Component
//    {
//        enum class Type : uint64_t
//        {
//            TRANSFORM   = BIT(0),
//            MODEL       = BIT(1),
//            LIGHT       = BIT(2),
//            SHADER      = BIT(3),
//            MATERIAL    = BIT(4),
//            MOVEMENT    = BIT(5),
//
//            MAX_COUNT   = BIT(63)
//        };
//
//        //enum class Type : uint32_t
//        //{
//        //    TRANSFORM   = 1 << 0,
//        //    MODEL       = 1 << 1,
//        //    LIGHT       = 1 << 2,
//        //    SHADER      = 1 << 3,
//        //    MATERIAL    = 1 << 4,
//        //    MOVEMENT    = 1 << 5,
//
//        //    MAX_COUNT = 1 << 31
//        //};
//
//        Type type;
//
//        //Component(Type _type) : type(_type) {}
//        //virtual ~Component() {}
//
//    };
//
//    
//
//
//
//
//
//}