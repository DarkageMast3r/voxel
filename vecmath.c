#include <math.h>
#include "vecmath.h"
#include <stdlib.h>
#include <stdio.h>

typedef unsigned int uint;

transform *transform_list;
uint transform_list_count = 0;
uint transform_list_max = 0;

vec4 vec4_scale_f(vec4 in, float s) {
	vec4 out = {
		in.x * s,
		in.y * s,
		in.z * s,
		in.w * s
	};
	return out;
}

vec4 vec4_add(vec4 a, vec4 b) {
	vec4 out = {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	};
	return out;
}

vec3 vec3_scale_f(vec3 in, float s) {
	vec3 out = {
		in.x * s,
		in.y * s,
		in.z * s
	};
	return out;
}

vec3 vec3_add(vec3 a, vec3 b) {
	vec3 out = {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	};
	return out;
}


uint transform_get_new() {
	if (transform_list_count == transform_list_max) {
		if (transform_list_max == 0) {
			transform_list_max = 1;
			transform_list = malloc(sizeof(*transform_list) * transform_list_max);
		} else {
			transform_list_max *= 2;
			transform *new = realloc(transform_list, sizeof(*transform_list) * transform_list_max);
			if (new == NULL) {
				printf("Failed to realloc!\n");
				transform_list_max /= 2;
			} else {
				transform_list = new;
			}
		}
	}

	transform_list_count++;

	return transform_list_count - 1;
}

transform *transform_get_by_id(uint id) {
	if (id >= transform_list_count) {
		printf("Transform ID out of range!\n");
		return NULL;
	}
	return &transform_list[id];
}

uint transform_create() {
	uint transform_id = transform_get_new();
	transform *transform = &transform_list[transform_id];

	transform->f[ 0] = 1.0f; // right
	transform->f[ 1] = 0.0f;
	transform->f[ 2] = 0.0f;
	transform->f[ 3] = 0.0f;
	transform->f[ 4] = 0.0f; // up
	transform->f[ 5] = 1.0f;
	transform->f[ 6] = 0.0f;
	transform->f[ 7] = 0.0f;
	transform->f[ 8] = 0.0f; // look
	transform->f[ 9] = 0.0f;
	transform->f[10] = 1.0f;
	transform->f[11] = 0.0f;
	transform->f[12] = 0.0f; // pos
	transform->f[13] = 0.0f;
	transform->f[14] = 0.0f;
	transform->f[15] = 1.0f;
	return transform_id;
}

void transform_print(uint transform_id) {
	transform *transform = transform_get_by_id(transform_id);
	printf(
"%3.3f %3.3f %3.3f %3.3f\n"
"%3.3f %3.3f %3.3f %3.3f\n"
"%3.3f %3.3f %3.3f %3.3f\n"
"%3.3f %3.3f %3.3f %3.3f\n",
		transform->f[ 0],
		transform->f[ 1],
		transform->f[ 2],
		transform->f[ 3],
		transform->f[ 4],
		transform->f[ 5],
		transform->f[ 6],
		transform->f[ 7],
		transform->f[ 8],
		transform->f[ 9],
		transform->f[10],
		transform->f[11],
		transform->f[12],
		transform->f[13],
		transform->f[14],
		transform->f[15]
	);
}

void transform_translate_f3(uint transform_id, float x, float y, float z) {
	transform *transform = transform_get_by_id(transform_id);
	transform->pos.x += x;
	transform->pos.y += y;
	transform->pos.z += z;
}


void transform_translate_vec3(uint transform_id, vec3 p) {
	transform *transform = transform_get_by_id(transform_id);
	transform->pos.x += p.x;
	transform->pos.y += p.y;
	transform->pos.z += p.z;
}

void transform_scale_f(uint transform_id, float s) {
	transform *transform = transform_get_by_id(transform_id);
	uint i;
	for (i = 0; i < 16; i++) {
		transform->f[i] *= s;
	}
}


void transform_rotate_reset(uint transform_id) {
	transform *transform = transform_get_by_id(transform_id);
	transform->right.x = 1.0f;
	transform->right.y = 0.0f;
	transform->right.z = 0.0f;
	transform->right.w = 0.0f;
	
	transform->up.x = 0.0f;
	transform->up.y = 1.0f;
	transform->up.z = 0.0f;
	transform->up.w = 0.0f;

	transform->look.x = 0.0f;
	transform->look.y = 0.0f;
	transform->look.z = 1.0f;
	transform->look.w = 0.0f;
}

void transform_rotate_x(uint transform_id, float r) {
	transform *transform = transform_get_by_id(transform_id);
	float c = cos(r);
	float s = sin(r);

	vec4 look_new = vec4_add(
		vec4_scale_f(transform->look, c),
		vec4_scale_f(transform->up, s)
	);
	vec4 up_new = vec4_add(
		vec4_scale_f(transform->up, c),
		vec4_scale_f(transform->look, -s)
	);
	transform->look = look_new;
	transform->up = up_new;
}


void transform_rotate_y(uint transform_id, float r) {
	transform *transform = transform_get_by_id(transform_id);
	float c = cos(r);
	float s = sin(r);

	vec4 right_new = vec4_add(
		vec4_scale_f(transform->right, c),
		vec4_scale_f(transform->look, s)
	);
	vec4 look_new = vec4_add(
		vec4_scale_f(transform->look, c),
		vec4_scale_f(transform->right, -s)
	);
	transform->look = look_new;
	transform->right = right_new;
}


void transform_rotate_z(uint transform_id, float r) {
	transform *transform = transform_get_by_id(transform_id);
	float c = cos(r);
	float s = sin(r);

	vec4 up_new = vec4_add(
		vec4_scale_f(transform->up, c),
		vec4_scale_f(transform->right, s)
	);
	vec4 right_new = vec4_add(
		vec4_scale_f(transform->right, c),
		vec4_scale_f(transform->up, -s)
	);
	transform->right = right_new;
	transform->up = up_new;
}
