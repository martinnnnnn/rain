#include "id_generator.h"


#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace rain
{
    unique_id IdGenerator::get_new_unique_id()
    {
        return unique_id(gen());
    }

    unique_id IdGenerator::get_new_unique_id(const std::string& _name)
    {
        static boost::uuids::uuid id;
        static boost::uuids::name_generator_latest name_generator(id);
        return name_generator(_name);
    }

    std::string IdGenerator::str(unique_id _id)
    {
        return boost::uuids::to_string(_id.data);
    }

}
