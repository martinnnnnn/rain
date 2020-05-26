#include "uuid.h"

#ifdef BOOST_INCLUDED
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#endif
namespace rain::core
{

    uuid uuid::nil()
    {
        return uuid{ {0} };
    }

    uuid uuid::random()
    {
#ifdef BOOST_INCLUDED
        static boost::uuids::random_generator random_gen;
        boost::uuids::uuid boost_id(random_gen());

        uuid id;
        memcpy(&id.data, &boost_id.data, sizeof(id.data));
        return id;
#else
        return uuid{};
#endif
    }

    uuid uuid::from_string(const std::string& str)
    {
#ifdef BOOST_INCLUDED
        static boost::uuids::string_generator string_gen;
        boost::uuids::uuid boost_id(string_gen(str));

        uuid id;
        memcpy(&id.data, &boost_id.data, sizeof(id.data));
        return id;
#else
        return uuid{};
#endif
    }

    uuid uuid::from_name(const std::string& name)
    {
#ifdef BOOST_INCLUDED
        static boost::uuids::uuid boost_name_id;
        static boost::uuids::name_generator_latest name_generator(boost_name_id);
        boost::uuids::uuid boost_id = name_generator(name);

        uuid id;
        memcpy(&id.data, &boost_id.data, sizeof(id.data));
        return id;
#else
        return uuid{};
#endif
    }

    std::string uuid::to_str(const uuid& id)
    {
#ifdef BOOST_INCLUDED
        boost::uuids::uuid boost_id;
        memcpy(&boost_id.data, &id.data, sizeof(boost_id.data));
        return boost::uuids::to_string(boost_id);
#else
        return "";
#endif
    }
}
