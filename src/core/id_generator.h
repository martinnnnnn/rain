#pragma once


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>


namespace rain
{
    struct uuid
    {
        uuid(boost::uuids::uuid boost_id)
            : data(boost_id)
        {}

        boost::uuids::uuid data;
    };

    struct IdGenerator
    {
        uuid get_new_unique_id();
        uuid get_new_unique_id(const std::string& _name);
        std::string str(uuid _id);

        boost::uuids::random_generator gen;

    };
}