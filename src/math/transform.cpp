#include "transform.h"

#include "serializer/archivist.h"

namespace rain
{
    void archive(Archivist* _p, Transform& _transform, const var_info& _info)
    {
        archive(_p, _transform.position, var_info("position"));
        archive(_p, _transform.orientation, var_info("orientation"));
    }
}