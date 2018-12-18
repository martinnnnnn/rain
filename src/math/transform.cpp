#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>


namespace rain
{
    const glm::mat4 Transform::get() const
    {
        return glm::translate(glm::mat4(1), position) * glm::mat4_cast(orientation) * glm::scale(glm::mat4(1), glm::vec3(scale));
    }
}