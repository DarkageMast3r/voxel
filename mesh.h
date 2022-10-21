#ifndef MESH_H
#define MESH_H

#include "vecmath.h"

typedef unsigned int uint;

typedef struct {
	float x, y, z;
//	float u, v, w;
	float brightness;
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

extern mesh *meshes;

extern uint *textures;
extern int texture_loc;
extern int chunk_loc;
extern int cam_pos_loc;

typedef struct {
	vec3 pos;
	vec3 look;
	vec3 right;
	vec3 up;
	vec3 rot;
} camera;

extern camera cam;


mesh *mesh_create(vertex *vertices, unsigned short vertex_count, uint *indices, unsigned short index_count);
mesh *mesh_copy(mesh *mesh);
void mesh_render();

#endif
