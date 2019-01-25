#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace rain::engine
{
    struct unique_id
    {
        unique_id()
            : data(boost::uuids::nil_uuid())
        {
            assert(data.is_nil());
        }

        unique_id(boost::uuids::uuid boost_id)
            : data(boost_id)
        {}

        boost::uuids::uuid data;
    };

    bool operator==(const unique_id& _a, const unique_id _b);

    struct IdGenerator
    {
        unique_id get_new_unique_id();
        unique_id get_new_unique_id(const std::string& _name);
        static std::string str(const unique_id _id);

        boost::uuids::random_generator gen;

    };
}
