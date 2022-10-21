#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "stb_image.h"
#include "mesh.h"

typedef unsigned int uint;

/*
typedef struct{
	float x, y, z;
	float u, v;
} vertex;

typedef struct {
	vertex *vertices;
	uint vertex_count;
	uint *indices;
	uint index_count;
	uint buffer_vertex, buffer_array, buffer_index;
} mesh;

typedef struct {
	float x, y, z;
} vec3;
*/

typedef struct {
	uint transform;
	vec3 rot;
} camera;





camera cam;

mesh player;
float player_x = 0.0f;
float player_y = 0.0f;

uint paused = 0;
uint *textures;


static int pressed[GLFW_KEY_LAST] = {0};


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
/*	printf(
		"%s key %s\n",
		action == GLFW_PRESS ? "Pressed" : "Released",
		key == GLFW_KEY_W ? "W" :
			(key == GLFW_KEY_S) ? "S" : 
			(key == GLFW_KEY_A) ? "A" :
			(key == GLFW_KEY_D) ? "D" : "a key"
	);
*/
	if (action == GLFW_PRESS) {
		pressed[key] = 1;
		if (key == GLFW_KEY_ESCAPE) {
			paused = !paused;
		}
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
	printf("Vertex size: %lu\n", sizeof(vertex));
	printf("Mesh size: %lu\n", sizeof(mesh));

	//glewExperimental = 1;
	if (!glfwInit()) {
		printf("Failed to initialize GLFW\n");
		return -1;
	}



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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	int texture_count_max;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_count_max);

	textures = malloc(sizeof(uint) * texture_count_max);
	printf("Able to use %d textures\n", texture_count_max);



	mesh_init();

//	transform *abcde;

	/*	
	{
		vertex vertices[4] = {
			{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f},
			{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
			{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f},
			{ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f}
		};
		uint indices[6] = {
			0, 1, 2,
			1, 2, 3
		};

//		float r = 0;
		int x, y, z;
		for (x = -5; x < 5; x++) {
			for (z = -5; z < 5; z++) {
				int y_max = (int) floor(sin(x * 1.5) + cos(z * 5.2)) + 5;
				for (y = -5; y < 5; y++) {
//					printf("%d %d %d\n", x, y, z);
					mesh *mesh = mesh_create(vertices, 4, indices, 6);
//					mesh *mesh = mesh_copy(std_mesh.cube);

					transform_translate_f3(mesh->transform, (float) x, (float) y, (float) z);
					transform_scale_f(mesh->transform, 1.0f);
				}
			}
		}
	}
	*/

	
	cam.rot.x = 0.0f;
	cam.rot.y = 0.0f;
	cam.rot.z = 0.0f;
//	printf("Val -1\n");
//	transform_print(abcde);
	cam.transform = transform_create();
//	printf("Val 0\n");
//	transform_print(abcde);
	
	transform_translate_f3(cam.transform, 0.0f, -3.0f, 0.0f);

//	printf("Val 1\n");
//	transform_print(abcde);

	char *shader_vertex_source = file_read("vertex.shader");
	char *shader_fragment_source = file_read("fragment.shader");

	uint shader = shader_create(shader_vertex_source, shader_fragment_source);
	render_shader = shader;

	free(shader_vertex_source);
	free(shader_fragment_source);

//	glUseProgram(shader);

//	printf("Val 2\n");
//	transform_print(abcde);

	stbi_set_flip_vertically_on_load(1);
	int width, height, channel_count;
	unsigned char *pixels = stbi_load("image.png", &width, &height, &channel_count, 0);
	printf("Loaded %d x %d image (%d channels)\n", width, height, channel_count);


	glGenTextures(1, textures);
	glGenTextures(1, textures + 1);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	glBindTexture(GL_TEXTURE_2D, 0);

	
	if (pixels !=  NULL) {
		stbi_image_free(pixels);
	}


	int texture_loc = glGetUniformLocation(shader, "img");
	//int block_loc = glGetUniformLocation(shader, "block");

//	int cam_pos_loc = glGetUniformLocation(shader, "cam_pos");
//	int cam_look_loc = glGetUniformLocation(shader, "cam_look");
//	int cam_right_loc = glGetUniformLocation(shader, "cam_right");
//	int cam_up_loc = glGetUniformLocation(shader, "cam_up");
	render_shader_transform_loc = glGetUniformLocation(shader, "transform");

	printf("Binding texture");
	//int texture_slot = 0;
//	glActiveTexture(GL_TEXTURE0 + texture_slot);
///	glBindTexture(GL_TEXTURE_2D, textures[0]);
//	glUniform1i(texture_loc, texture_slot);


	printf("Bound texture\n");

	double time = 0;

	int camera_loc = glGetUniformLocation(shader, "camera");

	double cursor_x, cursor_y;
	cursor_x = 512.0;
	cursor_y = 384.0;
	
	
//	transform_print(abcde);

	printf("\n\n");
	transform_create();

	while (!glfwWindowShouldClose(window)) {
		time += 0.01;

		int right = pressed[GLFW_KEY_D] - pressed[GLFW_KEY_A];
		int forward = pressed[GLFW_KEY_W] - pressed[GLFW_KEY_S];
		int up = pressed[GLFW_KEY_SPACE] - pressed[GLFW_KEY_LEFT_SHIFT];

		transform *cam_transform = transform_get_by_id(cam.transform);
		transform_translate_f3(
			cam.transform,
			-(cam_transform->look.x * forward * 0.1f + cam_transform->right.x * right * 0.1f + cam_transform->up.x * up * 0.1f),
			-(cam_transform->look.y * forward * 0.1f + cam_transform->right.y * right * 0.1f + cam_transform->up.y * up * 0.1f),
			-(cam_transform->look.z * forward * 0.1f + cam_transform->right.z * right * 0.1f + cam_transform->up.z * up * 0.1f)
		);

		printf("=====\n\n\n");
		printf("Transform cam: %d\n", cam.transform);
		transform_print(cam.transform);


		if (!paused) {
			double cursor_x_new, cursor_y_new;
			glfwGetCursorPos(window, &cursor_x_new, &cursor_y_new);
			glfwSetCursorPos(window, cursor_x, cursor_y);
	
			double dx = cursor_x_new - cursor_x;
			double dy = cursor_y_new - cursor_y;
			//cursor_x = cursor_x_new;
			//cursor_y = cursor_y_new;
	

			cam.rot.x += dy * -0.005f;
			cam.rot.y += dx * -0.005f;

			transform_rotate_reset(cam.transform);
			transform_rotate_y(cam.transform, cam.rot.y);
			transform_rotate_x(cam.transform, cam.rot.x);
		}


		
//		printf("%f %f %f\n",cam.transform.pos.x, cam.transform.pos.y, cam.transform.pos.z);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Setting up camera
		glUseProgram(shader);

		glUniformMatrix4fv(
			camera_loc	,
			1,
			GL_FALSE,
			(float *) &cam.transform
		);



		glUseProgram(0);





		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(texture_loc, 0);
				
/*
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);
*/

		mesh_render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}
