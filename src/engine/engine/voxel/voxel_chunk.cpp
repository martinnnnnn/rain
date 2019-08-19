#include "voxel_chunk.h"

#include "core/math/simplex_noise.h"
#include "engine/voxel/voxel_map.h"
#include <Windows.h>
#include "core/logger.h"
#include "gtx/transform.hpp"

namespace rain::engine
{

    void init(voxel_chunk* chunk, voxel_map* map, const glm::u32vec3& position)
    {
        chunk->map = map;
        chunk->position = position * CHUNK_SIZE;
        chunk->data = (voxel_cell*)calloc(CHUNK_SIZE_CUBED, sizeof(voxel_cell));

        for (u32 i = 0; i < CHUNK_SIZE; ++i)
        {
            for (u32 j = 0; j < CHUNK_SIZE; ++j)
            {
                for (u32 k = 0; k < CHUNK_SIZE; ++k)
                {
                    voxel_cell* cell = &chunk->data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED];

                    cell->distance = (core::simplex_noise::noise(float(i + chunk->position.x) / CHUNK_SIZE, float(j + chunk->position.y) / CHUNK_SIZE, float(k + chunk->position.z) / CHUNK_SIZE) + 1.0f) / 2.0f;
                    if (cell->distance > 0.7f)
                    {
                        cell->type = voxel_cell::Type::DIRT;
                        map->model_matrices.emplace_back(glm::scale(glm::translate(glm::mat4(1), glm::vec3{ i + chunk->position.x , j + chunk->position.y , k + chunk->position.z }), glm::vec3{ 1.0f, 1.0f, 1.0f }));
                    }
                    else
                    {
                        cell->type = voxel_cell::Type::EMPTY;
                    }
                }
            }
        }

        const glm::u32vec3 map_index{ chunk->position.x / CHUNK_SIZE, chunk->position.y / CHUNK_SIZE, chunk->position.z / CHUNK_SIZE };

        chunk->cXN = map_index.x > 0 ? &chunk->map->chunks[(map_index.x - 1) + (map_index.y * MAP_SIZE) + (map_index.z * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cYN = map_index.y > 0 ? &chunk->map->chunks[(map_index.x) + ((map_index.y - 1) * MAP_SIZE) + (map_index.z * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cZN = map_index.z > 0 ? &chunk->map->chunks[(map_index.x) + ((map_index.y) * MAP_SIZE) + ((map_index.z - 1) * MAP_SIZE_SQUARED)] : nullptr;

        chunk->cXP = map_index.x < MAP_SIZE - 1 ? &chunk->map->chunks[(map_index.x + 1) + ((map_index.y) * MAP_SIZE) + ((map_index.z) * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cYP = map_index.y < MAP_SIZE - 1 ? &chunk->map->chunks[(map_index.x) + ((map_index.y + 1) * MAP_SIZE) + ((map_index.z) * MAP_SIZE_SQUARED)] : nullptr;
        chunk->cZP = map_index.z < MAP_SIZE - 1 ? &chunk->map->chunks[(map_index.x) + ((map_index.y) * MAP_SIZE) + ((map_index.z + 1) * MAP_SIZE_SQUARED)] : nullptr;
    }

    voxel_cell* get_cell(voxel_chunk const * const chunk, const glm::u32vec3& position)
    {
        voxel_cell* cell = &chunk->data[position.x + position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARED];
        return cell;
    }

    bool is_cell_border(voxel_chunk const * const chunk, const glm::u32vec3& position)
    {
        voxel_cell* next = nullptr;

        // cXN
        next = nullptr;
        if (position.x > 0)
        {
            next = get_cell(chunk, glm::u32vec3{ position.x - 1, position.y, position.z });
        }
        else if (chunk->cXN != nullptr)
        {
            next = get_cell(chunk->cXN, glm::u32vec3{ CHUNK_SIZE - 1, position.y, position.z });
        }
        if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        {
            return true;
        }

        // cXP
        next = nullptr;
        if (position.x < CHUNK_SIZE - 1)
        {
            next = get_cell(chunk, glm::u32vec3{ position.x + 1, position.y, position.z });
        }
        else if (chunk->cXP != nullptr)
        {
            next = get_cell(chunk->cXP, glm::u32vec3{ 0, position.y, position.z });
        }
        if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        {
            return true;
        }

        //------------------------
        // cYN
        next = nullptr;
        if (position.y > 0)
        {
            next = get_cell(chunk, glm::u32vec3{ position.x, position.y - 1, position.z });
        }
        else if (chunk->cYN != nullptr)
        {
            next = get_cell(chunk->cYN, glm::u32vec3{ position.x, CHUNK_SIZE - 1, position.z });
        }
        if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        {
            return true;
        }

        // cYP
        next = nullptr;
        if (position.y < CHUNK_SIZE - 1)
        {
            next = get_cell(chunk, glm::u32vec3{ position.x, position.y + 1, position.z });
        }
        else if (chunk->cYP != nullptr)
        {
            next = get_cell(chunk->cYP, glm::u32vec3{ position.x, 0, position.z });
        }
        if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        {
            return true;
        }
        //------------------------
        // cZN
        next = nullptr;
        if (position.z > 0)
        {
            next = get_cell(chunk, glm::u32vec3{ position.x, position.y, position.z - 1 });
        }
        else if (chunk->cZN != nullptr)
        {
            next = get_cell(chunk->cZN, glm::u32vec3{ position.x, position.y, CHUNK_SIZE - 1 });
        }
        if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        {
            return true;
        }

        //------------------------
        // cZP
        next = nullptr;
        if (position.z < CHUNK_SIZE - 1)
        {
            next = get_cell(chunk, glm::u32vec3{ position.x, position.y, position.z + 1 });
        }
        else if (chunk->cZP != nullptr)
        {
            next = get_cell(chunk->cZP, glm::u32vec3{ position.x, position.y, 0 });
        }
        if (next == nullptr || next->type == voxel_cell::Type::EMPTY)
        {
            return true;
        }

        return false;
    }
}

//var MonotoneMesh = (function() {
//
//    function MonotonePolygon(c, v, ul, ur) {
//        this.color = c;
//        this.left = [[ul, v]];
//        this.right = [[ur, v]];
//    };
//
//    MonotonePolygon.prototype.close_off = function(v) {
//        this.left.push([this.left[this.left.length - 1][0], v]);
//        this.right.push([this.right[this.right.length - 1][0], v]);
//    };
//
//    MonotonePolygon.prototype.merge_run = function(v, u_l, u_r) {
//        var l = this.left[this.left.length - 1][0]
//            , r = this.right[this.right.length - 1][0];
//        if (l != = u_l) {
//            this.left.push([l, v]);
//            this.left.push([u_l, v]);
//        }
//        if (r != = u_r) {
//            this.right.push([r, v]);
//            this.right.push([u_r, v]);
//        }
//    };
//
//
//    return function(volume, dims) {
//        function f(i, j, k) {
//            return volume[i + dims[0] * (j + dims[1] * k)];
//        }
//        //Sweep over 3-axes
//        var vertices = [], faces = [];
//        for (var d = 0; d < 3; ++d) {
//            var i, j, k
//                , u = (d + 1) % 3   //u and v are orthogonal directions to d
//                , v = (d + 2) % 3
//                , x = new Int32Array(3)
//                , q = new Int32Array(3)
//                , runs = new Int32Array(2 * (dims[u] + 1))
//                , frontier = new Int32Array(dims[u])  //Frontier is list of pointers to polygons
//                , next_frontier = new Int32Array(dims[u])
//                , left_index = new Int32Array(2 * dims[v])
//                , right_index = new Int32Array(2 * dims[v])
//                , stack = new Int32Array(24 * dims[v])
//                , delta = [[0, 0], [0, 0]];
//            //q points along d-direction
//            q[d] = 1;
//            //Initialize sentinel
//            for (x[d] = -1; x[d] < dims[d]; ) {
//                // --- Perform monotone polygon subdivision ---
//                var n = 0
//                    , polygons = []
//                    , nf = 0;
//                for (x[v] = 0; x[v] < dims[v]; ++x[v]) {
//                    //Make one pass over the u-scan line of the volume to run-length encode polygon
//                    var nr = 0, p = 0, c = 0;
//                    for (x[u] = 0; x[u] < dims[u]; ++x[u], p = c) {
//                        //Compute the type for this face
//                        var a = (0 <= x[d] ? f(x[0], x[1], x[2]) : 0)
//                            , b = (x[d] < dims[d] - 1 ? f(x[0] + q[0], x[1] + q[1], x[2] + q[2]) : 0);
//                        c = a;
//                        if ((!a) == = (!b)) {
//                            c = 0;
//                        }
//                        else if (!a) {
//                            c = -b;
//                        }
//                        //If cell type doesn't match, start a new run
//                        if (p != = c) {
//                            runs[nr++] = x[u];
//                            runs[nr++] = c;
//                        }
//                    }
//                    //Add sentinel run
//                    runs[nr++] = dims[u];
//                    runs[nr++] = 0;
//                    //Update frontier by merging runs
//                    var fp = 0;
//                    for (var i = 0, j = 0; i < nf && j < nr - 2; ) {
//                        var p = polygons[frontier[i]]
//                            , p_l = p.left[p.left.length - 1][0]
//                            , p_r = p.right[p.right.length - 1][0]
//                            , p_c = p.color
//                            , r_l = runs[j]    //Start of run
//                            , r_r = runs[j + 2]  //End of run
//                            , r_c = runs[j + 1]; //Color of run
//                          //Check if we can merge run with polygon
//                        if (r_r > p_l && p_r > r_l && r_c == = p_c) {
//                            //Merge run
//                            p.merge_run(x[v], r_l, r_r);
//                            //Insert polygon into frontier
//                            next_frontier[fp++] = frontier[i];
//                            ++i;
//                            j += 2;
//                        }
//                        else {
//                            //Check if we need to advance the run pointer
//                            if (r_r <= p_r) {
//                                if (!!r_c) {
//                                    var n_poly = new MonotonePolygon(r_c, x[v], r_l, r_r);
//                                    next_frontier[fp++] = polygons.length;
//                                    polygons.push(n_poly);
//                                }
//                                j += 2;
//                            }
//                            //Check if we need to advance the frontier pointer
//                            if (p_r <= r_r) {
//                                p.close_off(x[v]);
//                                ++i;
//                            }
//                        }
//                    }
//                    //Close off any residual polygons
//                    for (; i < nf; ++i) {
//                        polygons[frontier[i]].close_off(x[v]);
//                    }
//                    //Add any extra runs to frontier
//                    for (; j < nr - 2; j += 2) {
//                        var r_l = runs[j]
//                            , r_r = runs[j + 2]
//                            , r_c = runs[j + 1];
//                        if (!!r_c) {
//                            var n_poly = new MonotonePolygon(r_c, x[v], r_l, r_r);
//                            next_frontier[fp++] = polygons.length;
//                            polygons.push(n_poly);
//                        }
//                    }
//                    //Swap frontiers
//                    var tmp = next_frontier;
//                    next_frontier = frontier;
//                    frontier = tmp;
//                    nf = fp;
//                }
//                //Close off frontier
//                for (var i = 0; i < nf; ++i) {
//                    var p = polygons[frontier[i]];
//                    p.close_off(dims[v]);
//                }
//                // --- Monotone subdivision of polygon is complete at this point ---
//
//                x[d]++;
//
//                //Now we just need to triangulate each monotone polygon
//                for (var i = 0; i < polygons.length; ++i) {
//                    var p = polygons[i]
//                        , c = p.color
//                        , flipped = false;
//                    if (c < 0) {
//                        flipped = true;
//                        c = -c;
//                    }
//                    for (var j = 0; j < p.left.length; ++j) {
//                        left_index[j] = vertices.length;
//                        var y = [0.0, 0.0, 0.0]
//                            , z = p.left[j];
//                        y[d] = x[d];
//                        y[u] = z[0];
//                        y[v] = z[1];
//                        vertices.push(y);
//                    }
//                    for (var j = 0; j < p.right.length; ++j) {
//                        right_index[j] = vertices.length;
//                        var y = [0.0, 0.0, 0.0]
//                            , z = p.right[j];
//                        y[d] = x[d];
//                        y[u] = z[0];
//                        y[v] = z[1];
//                        vertices.push(y);
//                    }
//                    //Triangulate the monotone polygon
//                    var bottom = 0
//                        , top = 0
//                        , l_i = 1
//                        , r_i = 1
//                        , side = true;  //true = right, false = left
//
//                    stack[top++] = left_index[0];
//                    stack[top++] = p.left[0][0];
//                    stack[top++] = p.left[0][1];
//
//                    stack[top++] = right_index[0];
//                    stack[top++] = p.right[0][0];
//                    stack[top++] = p.right[0][1];
//
//                    while (l_i < p.left.length || r_i < p.right.length) {
//                        //Compute next side
//                        var n_side = false;
//                        if (l_i == = p.left.length) {
//                            n_side = true;
//                        }
//                        else if (r_i != = p.right.length) {
//                            var l = p.left[l_i]
//                                , r = p.right[r_i];
//                            n_side = l[1] > r[1];
//                        }
//                        var idx = n_side ? right_index[r_i] : left_index[l_i]
//                            , vert = n_side ? p.right[r_i] : p.left[l_i];
//                        if (n_side != = side) {
//                            //Opposite side
//                            while (bottom + 3 < top) {
//                                if (flipped == = n_side) {
//                                    faces.push([stack[bottom], stack[bottom + 3], idx, c]);
//                                }
//                                else {
//                                    faces.push([stack[bottom + 3], stack[bottom], idx, c]);
//                                }
//                                bottom += 3;
//                            }
//                        }
//                        else {
//                            //Same side
//                            while (bottom + 3 < top) {
//                                //Compute convexity
//                                for (var j = 0; j < 2; ++j)
//                                    for (var k = 0; k < 2; ++k) {
//                                        delta[j][k] = stack[top - 3 * (j + 1) + k + 1] - vert[k];
//                                    }
//                                var det = delta[0][0] * delta[1][1] - delta[1][0] * delta[0][1];
//                                if (n_side == = (det > 0)) {
//                                    break;
//                                }
//                                if (det != = 0) {
//                                    if (flipped == = n_side) {
//                                        faces.push([stack[top - 3], stack[top - 6], idx, c]);
//                                    }
//                                    else {
//                                        faces.push([stack[top - 6], stack[top - 3], idx, c]);
//                                    }
//                                }
//                                top -= 3;
//                            }
//                        }
//                        //Push vertex
//                        stack[top++] = idx;
//                        stack[top++] = vert[0];
//                        stack[top++] = vert[1];
//                        //Update loop index
//                        if (n_side) {
//                            ++r_i;
//                        }
//                        else {
//                            ++l_i;
//                        }
//                        side = n_side;
//                    }
//                }
//            }
//        }
//        return { vertices:vertices, faces : faces };
//    }
//})();
//
//if (exports) {
//    exports.mesher = MonotoneMesh;
//}