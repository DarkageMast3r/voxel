#ifndef VECMATH_H
#define VECMATH_H

typedef unsigned int uint;

typedef struct {
	float x, y, z;
} vec3;

typedef struct {
	float x, y, z, w;
} vec4;

typedef union {
	float f[16];
	struct {
		vec4 right, up, look, pos;
	};
} transform;



vec4 vec4_scale_f(vec4 in, float s);
vec4 vec4_add(vec4 a, vec4 b);

vec3 vec3_scale_f(vec3 in, float s);
vec3 vec3_add(vec3 a, vec3 b);

uint transform_create();
transform *transform_get_by_id(uint id);
void transform_translate_f3(uint transform_id, float x, float y, float z);
void transform_translate_vec3(uint transform_id, vec3 p);
void transform_scale_f(uint transform_id, float s);
void transform_print(uint transform_id);


// all rotations are clockwise
void transform_rotate_reset(uint transform_id);
void transform_rotate_x(uint transform_id, float r); // rotates along the right vector
void transform_rotate_y(uint transform_id, float r); // rotates along the up vector
void transform_rotate_z(uint transform_id, float r); // rotates along the look vector


#endif
