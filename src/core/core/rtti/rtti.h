#pragma once

#include <string>

namespace rain::core
{
    class Rtti
    {
    public:
        Rtti(const std::string name, const Rtti* base);
        ~Rtti();

        const std::string get_name() const;

        bool is_exactly(const Rtti& type) const;
        bool is_derived(const Rtti& type) const;

    private:
        std::string name;
        const Rtti* base;
    };

}

#define RAIN_DECLARE_RTTI \
public: \
    static const rain::core::Rtti TYPE; \
    virtual const rain::core::Rtti& get_type () const { return TYPE; }

#define RAIN_IMPLEMENT_RTTI(nsname,classname,baseclassname) \
    const rain::core::Rtti classname::TYPE(#nsname"."#classname,&baseclassname::TYPE)