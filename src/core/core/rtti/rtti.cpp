#include "rtti.h"

namespace rain::core
{
    Rtti::Rtti(const std::string name, const Rtti* base)
        : name(name)
        , base(base)
    {
    }

    Rtti::~Rtti()
    {
    }

    const std::string Rtti::get_name() const
    {
        return name;
    }

    bool Rtti::is_exactly(const Rtti& type) const
    {
        return &type == this;
    }

    bool Rtti::is_derived(const Rtti& type) const
    {
        const Rtti* search = this;
        while (search)
        {
            if (search == &type)
            {
                return true;
            }
            search = search->base;
        }
        return false;
    }
}