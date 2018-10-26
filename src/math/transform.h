

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace rain
{
    struct Transform
    {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
    };
}