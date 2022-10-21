#version 330 core

layout(location = 0) out vec4 color;
in vec3 uv;
in float depth;
in float fbrightness;

uniform sampler2D img;
uniform sampler3D chunk;

void main() {
	vec4 clr = texture(img, uv.xy);
	vec4 clr_chunk = texture3D(chunk, uv);


//	color = vec4(uv, 1.0f);
	if (clr_chunk.a <= 0.0f) {
		discard;
	}
	color = clr_chunk * vec4(vec3(fbrightness), 1.0f);
	
}

