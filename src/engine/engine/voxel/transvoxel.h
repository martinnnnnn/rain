#pragma once

#include "glm.hpp"
#include <vector>
#include <algorithm>
#include "float.h"

#include "core/core.h"
#include "transvoxel_tables.h"
#include "vox_cell.h"
#include "vox_constants.h"

namespace rain::engine::voxel
{
    struct vox_block;
    struct vox_map;

    void transvoxel(vox_block* block);
    void transvoxel(vox_map* map);
}
