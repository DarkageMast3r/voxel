#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 uvCoord;

out vec3 uv;
// out vec2 depth;

uniform mat4x4 transform;
uniform mat4x4 camera;
/*
uniform vec3 cam_pos;
uniform vec3 cam_look;
uniform vec3 cam_right;
uniform vec3 cam_up;
*/

void main() {
	uv = uvCoord;
	//vec3 pos_world = position + transform[3].xyz;
	


	vec3 pos_world = vec3(
		dot(position, transform[0].xyz),
		dot(position, transform[1].xyz),
		dot(position, transform[2].xyz)
	);
	pos_world += transform[3].xyz;







	

//	vec3 pos_world = position.xyz;	
//	vec3 pos_world = (transform * vec4(position, 1.0f)).xyz;
//	vec3 pos_project = pos_world;


	pos_world -= camera[3].xyz;
	
	vec3 pos_project = vec3(
		dot(pos_world, camera[0].xyz),
		dot(pos_world, camera[1].xyz),
		dot(pos_world, camera[2].xyz)
	);



	gl_Position = vec4(
		pos_project.xy / max(pos_project.z, 0.0000f),
		pos_project.z / 256.0f,
		1.0f
	);
}
