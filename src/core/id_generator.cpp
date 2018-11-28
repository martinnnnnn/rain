#include "id_generator.h"


#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace rain
{
    uuid IdGenerator::get_new_unique_id()
    {
        return uuid(gen());
    }

    uuid IdGenerator::get_new_unique_id(const std::string& _name)
    {
        static boost::uuids::uuid id;
        static boost::uuids::name_generator_latest name_generator(id);
        return name_generator(_name);
    }

    std::string IdGenerator::str(uuid _id)
    {
        return boost::uuids::to_string(_id.data);
    }

}
