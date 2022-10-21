#ifndef MESH_H
#define MESH_H

#include "vecmath.h"

typedef unsigned int uint;

typedef struct {
	float x, y, z;
	float u, v, w;
} vertex;

typedef struct {
	uint transform;
	vertex *vertices;
	uint *indices;
	unsigned short vertex_count;
	unsigned short index_count;
	uint buffer_vertex, vertex_array, buffer_index;
} mesh;


typedef struct {
	mesh *cube;
} std_mesh_s;

extern std_mesh_s std_mesh;

extern uint render_shader;
extern uint render_shader_transform_loc;


void mesh_init();
mesh *mesh_create(vertex *vertices, unsigned short vertex_count, uint *indices, unsigned short index_count);
mesh *mesh_copy(mesh *mesh);
void mesh_render();

#endif
