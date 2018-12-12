#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>

#include "win32/win32_application.h"


namespace rain
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

    struct IdGenerator
    {
        unique_id get_new_unique_id();
        unique_id get_new_unique_id(const std::string& _name);
        std::string str(unique_id _id);

        boost::uuids::random_generator gen;

    };
}

#define RAIN_NEW_ID rain::Application::get().id_generator->get_new_unique_id()