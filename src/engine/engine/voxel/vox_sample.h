#pragma once



namespace rain::engine::voxel
{
    struct vox_block;

    struct vox_sample
    {
        vox_sample() : dist(0), x(0), y(0), z(0), owner(nullptr) {}

        i8 dist;
        u8 x;
        u8 y;
        u8 z;
        vox_block* owner;
    };
}