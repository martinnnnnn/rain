#include "quick_hull.h"

#define QUICKHULL_IMPLEMENTATION
//#define QUICKHULL_DEBUG
#include "quick_hull_lib.h"

namespace rain::math
{

    // TODO (martin) : for now I use someone else's implementation of quick_hull,
    // I will try to roll out my own eventually
    void quick_hull(vec3 const *  vertices, const u32 vertices_count, mesh* output)
    {
        qh_vertex_t * lib_vertices = (qh_vertex_t *)vertices;
        qh_mesh_t lib_mesh = qh_quickhull3d(lib_vertices, vertices_count);

        output->vertices = (vec3*)malloc(lib_mesh.nvertices * sizeof(vec3));
        output->vertices = (vec3*)lib_mesh.vertices;
        output->vertices_count = lib_mesh.nvertices;

        output->vertices_indices = (u32*)malloc(lib_mesh.nindices * sizeof(u32));
        output->vertices_indices = lib_mesh.indices;
        output->vertices_indices_count = lib_mesh.nindices;

        output->normals = (vec3*)lib_mesh.normals;
        output->normals_count = lib_mesh.nnormals; 

        output->normals_indices = lib_mesh.normalindices;
        output->normal_indices_count = lib_mesh.nnormals;
    }
}