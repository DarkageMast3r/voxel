#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 uvCoord;
layout(location = 2) in float brightness;

out float fbrightness;
out vec3 uv;
// out vec2 depth;

uniform mat4x4 transform;
uniform vec3 cam_pos;
uniform vec3 cam_look;
uniform vec3 cam_right;
uniform vec3 cam_up;

void main() {
	uv = (position + transform[3].xyz) / vec3(256.0f);
	fbrightness = brightness;
	
	
	vec3 pos_local = transform[3].xyz
		+ transform[0].xyz * position.x
		+ transform[1].xyz * position.y
		+ transform[2].xyz * position.z;
		

	pos_local -= cam_pos;
	//vec3 pos_local = position - cam_pos;

	vec3 pos_project = vec3(
		dot(pos_local, cam_right),
		dot(pos_local, cam_up),
		dot(pos_local, cam_look)
	);
	//depth = pos_project.z;
	gl_Position = vec4(
		pos_project.xy / max(pos_project.z, 0.0000f),
		pos_project.z / 25600.0f,
		1.0f
	);
}
