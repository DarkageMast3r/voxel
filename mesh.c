#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stddef.h>


#include <stdio.h>


#include "vecmath.h"
#include "mesh.h"



mesh *meshes;
static uint mesh_count = 0;
static uint mesh_count_max = 0;

uint render_shader;
uint render_shader_transform_loc;
std_mesh_s std_mesh;

uint *textures;
int texture_loc;
int chunk_loc;
int cam_pos_loc;


camera cam = {
	.pos = {0.0f, 0.0f, 0.0f},
	.rot = {0.0f, 0.0f, 0.0f},
	.look =  {0.0f, 0.0f, 1.0f},
	.up =    {0.0f, 1.0f, 0.0f},
	.right = {1.0f, 0.0f, 0.0f}
};



mesh *mesh_get() {
	if (mesh_count >= mesh_count_max) {
		if (mesh_count_max == 0) {
			mesh_count_max = 1;
			meshes = malloc(mesh_count_max * sizeof(*meshes));
		} else {
			mesh_count_max *= 2;
			mesh *new = realloc(meshes, mesh_count_max * sizeof(*meshes));
			if (new == NULL) {
				printf("Failed to realloc!\n");
				mesh_count_max /= 2;
				// TODO write util file for this list stuff
				// TODO make realloc try smaller sizer
			} else {
				meshes = new;
			}
		}
	}

	mesh *mesh = &meshes[mesh_count];
	mesh_count++;
	return mesh;
}

mesh *mesh_create(vertex *vertices, unsigned short vertex_count, uint *indices, unsigned short index_count) {
	uint buffer_vertex, vertex_array, buffer_index;

	glGenBuffers(1, &buffer_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_index);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(*indices), indices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &buffer_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(*vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*vertices), (const void *) offsetof(vertex, x));
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(*vertices), (const void *) offsetof(vertex, u));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(*vertices), (const void *) offsetof(vertex, brightness));
	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#ifdef DEBUG
	printf("Generated mesh with %d vertices, %d indices.\n", vertex_count, index_count);
#endif

	mesh *mesh = mesh_get();

#ifdef DEBUG
	printf("%d / %d meshes.\n", mesh_count, mesh_count_max);
#endif
	
//	transform_create((transform*) &mesh->transform);
	mesh->transform = transform_create();

	mesh->vertices = vertices;
	mesh->indices = indices;
	mesh->vertex_count = vertex_count;
	mesh->index_count = index_count;
	mesh->buffer_vertex = buffer_vertex;
	mesh->vertex_array = vertex_array;
	mesh->buffer_index = buffer_index;

	return mesh;
}

void mesh_render() {
	uint i;

	for (i = 0; i < mesh_count; i++) {
		mesh *mesh = &meshes[i];

		glUniformMatrix4fv(
                        render_shader_transform_loc,
                        1,
                        GL_FALSE,
                        (float *) transform_get_by_id(mesh->transform)
                );


		glBindVertexArray(mesh->vertex_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffer_index);
		glUniform3f(cam_pos_loc, cam.pos.x, cam.pos.y, cam.pos.z);
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);		
	}
}

mesh *mesh_copy(mesh *original) {
	mesh *copy = mesh_get();
	copy->vertices = original->vertices;
	copy->indices = original->indices;
	copy->vertex_count = original->vertex_count;
	copy->index_count = original->index_count;
	copy->buffer_vertex = original->buffer_vertex;
	copy->vertex_array = original->vertex_array;
	copy->buffer_index = original->buffer_index;
	copy->transform = transform_create();

	//	transform_create(copy->transform);

	return copy;
}

