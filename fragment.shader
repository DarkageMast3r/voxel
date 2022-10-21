#version 330 core

layout(location = 0) out vec4 color;
in vec3 uv;
in float depth;

uniform sampler2D img;


void main() {
	
	vec4 clr = texture(img, uv.xy);
//	vec4 clr_block = vec4(texture(block, uv).r, 1.0f, 1.0f, 1.0f);
//	color = mix(
//		vec4(uv, 1.0f),
//		clr,
//		clr.a
//	);


	color = clr;




















	//float r = texture(block, uv);
	//color = vec4(uv.rgb, 0.5f);
}

