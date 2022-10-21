#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <math.h>
#include "mesh.h"
#include "vecmath.h"

#include "stb_image.h"

typedef unsigned int uint;


mesh player;
mesh player2;
float player_x = 0.0f;
float player_y = 0.0f;



//uint *textures;

unsigned char block_colours[12]  = {
	  0,   0,   0,   0,
	  0, 127,  14, 255,
	126,  51,   0, 255
};

int pressed[GLFW_KEY_LAST] = {0};



char *file_read(char *path) {
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		printf("Failed to open file '%s'\n", path);
		return NULL;
	}

	fseek(file, 0, SEEK_END);

	long int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	char *source = malloc((length) * sizeof(char));
	fread(source, length * sizeof(char), 1, file);
	source[length] = 0;

	printf("File size: %ld bytes\n", length);

	return source;

}


uint shader_compile(char *source, uint type) {
	uint shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, (const char* const*) &source, NULL);
	glCompileShader(shader_id);

	int result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

		char *message;
		message = malloc(sizeof(*message) * (length+1));
		glGetShaderInfoLog(shader_id, length, &length, message);
		printf(
			"%s shader compilation error. Message:\n%s\n\n",
			type == GL_VERTEX_SHADER ? "Vertex" : "Fragment",
			message
		);
	}


	return shader_id;
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		pressed[key] = 1;
	} else if (action == GLFW_RELEASE) {
		pressed[key] = 0;
	}
}


uint shader_create(char *shader_vertex_source, char* shader_fragment_source) {
	uint program = glCreateProgram();

	uint shader_vertex = shader_compile(shader_vertex_source, GL_VERTEX_SHADER);
	uint shader_fragment = shader_compile(shader_fragment_source, GL_FRAGMENT_SHADER);
	
	glAttachShader(program, shader_vertex);
	glAttachShader(program, shader_fragment);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(shader_vertex);
	glDeleteShader(shader_fragment);

	return program;
}


int main() {	
	
	//glewExperimental = 1;
	printf("Hello world!\n");
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		return -1;
	}

	printf("initialized glfw\n");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Project 1", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create window!\n");
		glfwTerminate();
		return -1;
	}
		
	glfwMakeContextCurrent(window);
	printf("Created window\n");

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	int texture_count_max;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_count_max);

	textures = malloc(sizeof(uint) * texture_count_max);
	printf("Able to use %d textures\n", texture_count_max);

/*
	vertex vertices[4] = {
		{-0.5f, -0.5f, 1.0f,  0.0f, 0.0f},
		{-0.5f,  0.5f, 1.0f,  0.0f, 1.0f},
		{ 0.5f,  0.5f, 3.0f,  1.0f, 1.0f},
		{ 0.5f, -0.5f, 1.0f,  1.0f, 0.0f}
	};
	
	uint indices[6] = {
		0, 1, 2,
		0, 2, 3
	};
	printf("Generating new mesh\n");
	mesh_create(vertices, 4, indices, 6, &player);
*/

	printf("abc\n");	
	{
		int chunk_x, chunk_y, chunk_z;	
	
		float r = 0;
		int x_min = 0;
		int x_max = 15;
		int y_min = 0;
		int y_max = 15;
		int z_min = 0;
		int z_max = 15;
		for (chunk_x = x_min; chunk_x <= x_max; chunk_x++) {
			for (chunk_y = y_min; chunk_y <= y_max; chunk_y++) {
				for (chunk_z = z_min; chunk_z <= z_max; chunk_z++) {
					r += 0.1f;
					uint vertex_count = 64*3;
					uint index_count = 96*3;
					vertex *vertices;
					vertices = malloc(vertex_count * sizeof(*vertices));
	
					uint *indices;
					indices = malloc(index_count * sizeof(*indices));
	
	

					uint y;
					for (y = 0; y < 16; y++) {
							
						uint i = y*4;
						vertices[i].x =   0.0f;
						vertices[i].y =   1.0f * (float) y;
						vertices[i].z =   0.0f;
						vertices[i].brightness = 1.0f;

						vertices[i+1].x =  16.0f;
						vertices[i+1].y =   1.0f * (float) y;
						vertices[i+1].z =   0.0f;
						vertices[i+1].brightness = 1.0f;

						vertices[i+2].x =   0.0f;;
						vertices[i+2].y =   1.0f * (float) y;
						vertices[i+2].z =  16.0f;
						vertices[i+2].brightness = 1.0f;
			
						vertices[i+3].x =  16.0f;
						vertices[i+3].y =   1.0f * (float) y;
						vertices[i+3].z =  16.0f;
						vertices[i+3].brightness = 1.0f;



					}

					for (y = 0; y < 16; y++) {
						uint i = y*6;
						uint j = y*4;
						indices[i  ] = j;
						indices[i+1] = j+1;
						indices[i+2] = j+2;
						indices[i+3] = j+1;
						indices[i+4] = j+2;
						indices[i+5] = j+3;
					}



			
					uint x;
					for (x = 0; x < 16; x++) {
						uint i = x*4 + 64;
						vertices[i].x =   1.0f * (float) x;
						vertices[i].y =   0.0f; 
						vertices[i].z =   0.0f;
						vertices[i].brightness = 0.8f;

						vertices[i+1].x =   1.0f * (float) x;
						vertices[i+1].y =  16.0f;
						vertices[i+1].z =   0.0f;
						vertices[i+1].brightness = 0.8f;

						vertices[i+2].x =   1.0f * (float) x;
						vertices[i+2].y =   0.0f;
						vertices[i+2].z =  16.0f;
						vertices[i+2].brightness = 0.8f;
			
						vertices[i+3].x =   1.0f * (float) x;
						vertices[i+3].y =  16.0f;
						vertices[i+3].z =  16.0f;
						vertices[i+3].brightness = 0.8f;
					}


					for (x = 0; x < 16; x++) {
						uint i = x*6 + 96;
						uint j = x*4 + 64;
						indices[i  ] = j;
						indices[i+1] = j+1;
						indices[i+2] = j+2;
						indices[i+3] = j+1;
						indices[i+4] = j+2;
						indices[i+5] = j+3;
					}


					
					uint z;
                                        for (z = 0; z < 16; z++) {
                                                uint i = z*4 + 128;
						vertices[i].x = 0.0f;
						vertices[i].y = 0.0f;
						vertices[i].z = 1.0f * (float) z;
						vertices[i].brightness = 0.6f;

                                                vertices[i+1].x =   0.0f;
                                                vertices[i+1].y =  16.0f;
                                                vertices[i+1].z =   1.0f * (float) z;
                                                vertices[i+1].brightness = 0.6f;

                                                vertices[i+2].x =  16.0f;
                                                vertices[i+2].y =   0.0f;
                                                vertices[i+2].z =   1.0f * (float) z;
                                                vertices[i+2].brightness = 0.6f;

                                                vertices[i+3].x =  16.0f;
                                                vertices[i+3].y =  16.0f;
                                                vertices[i+3].z =   1.0f * (float) z;
                                                vertices[i+3].brightness = 0.6f;

                                        }

                                        printf("Did vertices\n");
                                        for (z = 0; z < 16; z++) {
                                                uint i = z*6 + 192;
                                                uint j = z*4 + 128;
                                                indices[i  ] = j;
                                                indices[i+1] = j+1;
                                                indices[i+2] = j+2;
                                                indices[i+3] = j+1;
                                                indices[i+4] = j+2;
                                                indices[i+5] = j+3;
                                        }


					player2 = player;
					player = *mesh_create(vertices, vertex_count, indices, index_count);
/*
					transform_rotate_x(
						player.transform,
						r
					);
*/
					transform_translate_f3(
						player.transform,
						(float) chunk_x * 16.0f,
						(float) chunk_y * 16.0f,
						(float) chunk_z * 16.0f
					);
				}
			}
		}
	}
	


	char *shader_vertex_source = file_read("vertex.shader");
	char *shader_fragment_source = file_read("fragment.shader");

	uint shader = shader_create(shader_vertex_source, shader_fragment_source);

	free(shader_vertex_source);
	free(shader_fragment_source);

	glUseProgram(shader);


	
	
	stbi_set_flip_vertically_on_load(1);
	int width, height, channel_count;
	unsigned char *pixels = stbi_load("image.png", &width, &height, &channel_count, 0);
	printf("Loaded %d x %d image (%d channels)\n", width, height, channel_count);


	glGenTextures(1, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels

	);
//	glBindTexture(GL_TEXTURE_2D, 0);
	


	glGenTextures(1, &textures[1]);
	glBindTexture(GL_TEXTURE_3D, textures[1]);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	printf("Got here\n");

	unsigned char *chunk_img;



	printf("Got var\n");
	{

		clock_t start_t, end_t;
		start_t = clock();

		uint size = 256;
		chunk_img = malloc(sizeof(*chunk_img) * size*size*size*4);

		uint x, y, z;
		for (z = 0; z < size; z++) {
			for (y = 0; y < size; y++) {
				for (x = 0; x < size; x++) {


					uint index = 4*(x + size*(z + size * y));
					float height = cos((double) x)*24.0f + cos((double) z)*32.0f + 64.0f;
					uint block = 0;
					if ((float) y <= height) {
						block = 1;
					}
					if ((float) y <= height - 4) {
						block = 2;
					}
				
					/*	
					chunk_img[index] = 255;
					chunk_img[index+1] = 255;
					chunk_img[index+2] = 255;
					chunk_img[index+3] = 255;
					*/
					

					chunk_img[index]   = block_colours[block*4 + 0];
					chunk_img[index+1] = block_colours[block*4 + 1];
					chunk_img[index+2] = block_colours[block*4 + 2];
					chunk_img[index+3] = block_colours[block*4 + 3];
/*
					chunk_img[index] = x;
					chunk_img[index+1] = y;
					chunk_img[index+2] = z;
					chunk_img[index+3] = 255; 
*/
				}
			}
		}

		end_t = clock();
		printf("Clock speed: %ld\n", CLOCKS_PER_SEC);
		printf("Generating map tex: %ld cycles\n", end_t - start_t);
		start_t = clock();


		glTexImage3D(
			GL_TEXTURE_3D,
			0,
			GL_RGBA8,
			256, 256, 256,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			chunk_img
		);

		end_t = clock();
		printf("Sending map tex: %ld cycles\n", end_t - start_t);

		printf("Send img\n");
		glEnable(GL_TEXTURE_3D);
		printf("Enabled 3D\n");
	}
	printf("Set up img\n");



//	glBindTexture(GL_TEXTURE_3D, 0);



	if (pixels !=  NULL) {
		stbi_image_free(pixels);
	}


	texture_loc = glGetUniformLocation(shader, "img");
	printf("TEXTURE LOC: %d (%d)\n", texture_loc, glGetUniformLocation(shader, "img"));
	chunk_loc = glGetUniformLocation(shader, "chunk");
	cam_pos_loc = glGetUniformLocation(shader, "cam_pos");
	int cam_look_loc = glGetUniformLocation(shader, "cam_look");
	int cam_right_loc = glGetUniformLocation(shader, "cam_right");
	int cam_up_loc = glGetUniformLocation(shader, "cam_up");
	printf("CHUNK LOC: %d\n", chunk_loc);
	printf("CAM POS LOc: %d\n", cam_pos_loc);

	printf("Binding texture");
	int texture_slot = 0;
//	glActiveTexture(GL_TEXTURE0 + texture_slot);
///	glBindTexture(GL_TEXTURE_2D, textures[0]);
//	glUniform1i(texture_loc, texture_slot);


	printf("Bound texture\n");

	double time = 0;

	int transform_loc = glGetUniformLocation(shader, "transform");
	render_shader_transform_loc = transform_loc;


	clock_t start_t = clock();
	while (!glfwWindowShouldClose(window)) {
		time += 0.01;

		int right = pressed[GLFW_KEY_D] - pressed[GLFW_KEY_A];
		int forward = pressed[GLFW_KEY_W] - pressed[GLFW_KEY_S];
		int up = pressed[GLFW_KEY_SPACE] - pressed[GLFW_KEY_LEFT_SHIFT];

		int speed = (!pressed[GLFW_KEY_RIGHT_CONTROL] * 9 + 1) * (pressed[GLFW_KEY_LEFT_CONTROL] * 9 + 1);

		cam.pos.x += speed * right * 0.1f;
		cam.pos.y += speed * up * 0.1f;
		cam.pos.z += speed * forward * 0.1f;



		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUseProgram(shader);


		glUniform3f(cam_pos_loc, cam.pos.x, cam.pos.y, cam.pos.z);
		glUniform3f(cam_look_loc,  cam.look.x,  cam.look.y,  cam.look.z);
		glUniform3f(cam_right_loc, cam.right.x, cam.right.y, cam.right.z);
		glUniform3f(cam_up_loc,    cam.up.x,    cam.up.y,    cam.up.z);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(texture_loc, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_3D, textures[1]);
		glUniform1i(chunk_loc, 1);

		
		glBindVertexArray(player.vertex_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, player.buffer_index);
		glDrawElements(GL_TRIANGLES, player.index_count, GL_UNSIGNED_INT, 0);

		mesh_render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		clock_t end_t = clock();
		clock_t cycles = end_t - start_t;
		float dt = (float) cycles / CLOCKS_PER_SEC;
		printf("%.1f FPS\n", 1.0f / dt);
		start_t = end_t;
	}
	
	return 0;
}
