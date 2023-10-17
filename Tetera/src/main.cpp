/*  by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) */

// TETERA DE OSCAR JULIÁN PONTE - GI - oscar.julian

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//include some custom code files
#include "glfunctions.h" //include all OpenGL stuff
#include "Shader.h" // class to compile shaders

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

//global variables to help us do things
int g_ViewportWidth = 512; int g_ViewportHeight = 512;	// Default window size, in pixels
double mouse_x, mouse_y;	//variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f);		// background colour - a GLM 3-component vector

GLuint g_simpleShader = 0; //shader identifier
GLuint g_Vao = 0; //vao
GLuint g_NumTriangles = 0; //  Numbre of triangles we are painting.

struct Mesh {
	GLuint Vao = 0;
	GLuint NumTriangles = 0;
};

struct Obj {
	std::string mesh_path = "";
	std::pair<std::string, std::string> shader_path = { "", "" };
	GLuint simpleShader = 0;
	Mesh*  mesh;
	vec4   color;
	mat4   model;
	mat4	projection;
};

struct Camera
{
	vec3 eye = vec3(0.0, 0.0, 0.0); // the position of your camera, in world space
	vec3 center = vec3(0.0, 0.0, -1.0); // where you want to look at, in world space
	vec3 up = vec3(0.0, 1.0, 0.0); // probably glm::vec3(0,1,0)
};

Camera cam;

mat4 view_matrix = glm::lookAt(
	cam.eye, // the position of your camera, in world space
	cam.center, // where you want to look at, in world space
	cam.up // probably glm::vec3(0,1,0)
);


vec3 front = cam.center - cam.eye;
vec3 normalizefront = glm::normalize(front);
vec3 side = glm::cross(cam.up, normalizefront);
vec3 normalizeside = glm::normalize(side);
vec3 top = glm::cross(normalizeside, normalizefront);

std::vector<Obj> objs;
int selected = -1;
std::map<std::string, Mesh> meshes;
std::map<std::pair<std::string, std::string>, GLuint> shaders;

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load() {
	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************
	std::string basepath = "assets/";
	for (size_t i = 0; i < objs.size(); i++) {
		Obj& o = objs[i];

		//load the shader
		if (!shaders.count(o.shader_path)) {
			std::string vs = o.shader_path.first;
			std::string fs = o.shader_path.second;
			Shader simpleShader(vs.c_str(), fs.c_str());
			shaders[o.shader_path] = simpleShader.program;
		}
		o.simpleShader = shaders[o.shader_path];

		std::string inputfile = basepath + o.mesh_path;

		if (!meshes.count(inputfile)) {
			std::vector< tinyobj::shape_t		>	shapes;
			std::vector< tinyobj::material_t	>	materials;
			std::string err;
			bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), basepath.c_str());

			Mesh mesh;
			// Create the VAO where we store all geometry (stored in g_Vao)
			mesh.Vao = gl_createAndBindVAO();

			//create vertex buffer for positions, colors, and indices, and bind them to shader
			gl_createAndBindAttribute(shapes[0].mesh.positions.data(), shapes[0].mesh.positions.size() * sizeof(float), o.simpleShader, "a_vertex", 3);
			//gl_createAndBindAttribute(shapes[0].mesh.normals.data(), shapes[0].mesh.normals.size() * sizeof(float), o.simpleShader, "a_color", 3);
			gl_createIndexBuffer(shapes[0].mesh.indices.data(), shapes[0].mesh.indices.size() * sizeof(unsigned int));

			//unbind everything
			gl_unbindVAO();

			//store number of triangles (use in draw())
			mesh.NumTriangles = shapes[0].mesh.indices.size() / 3;

			meshes[inputfile] = mesh;
		}
		o.mesh = &meshes[inputfile];

	}

}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw() {
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.2, .2, .2, 3.0);
	glCullFace(GL_CW);

	for (Obj o : objs) {

		o.projection = perspective(
			60.0f, // Field of view
			1.0f, // Aspect ratio
			0.1f, // near plane (distance from camera)
			50.0f // Far plane (distance from camera)
		);

		GLuint view_loc = glGetUniformLocation(o.simpleShader, "u_view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

		GLuint projection_loc = glGetUniformLocation(o.simpleShader, "u_projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(o.projection));

		// activate shader
		glUseProgram(o.simpleShader);

		//bind the geometry
		gl_bindVAO(o.mesh->Vao);

		o.color.r = sin(glfwGetTime()) * .5 + .5;


		GLuint color_loc = glGetUniformLocation(o.simpleShader, "u_color");
		glUniform4f(color_loc, o.color.r, o.color.g, o.color.b, o.color.a);


		GLuint model_loc = glGetUniformLocation(o.simpleShader, "u_model");
		glUniformMatrix4fv(model_loc, 1, false, glm::value_ptr(o.model));


		// Draw to screen
		glDrawElements(GL_TRIANGLES, 3 * o.mesh->NumTriangles, GL_UNSIGNED_INT, 0);
	}
}

// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
	if (key == GLFW_KEY_R && action & (GLFW_PRESS | GLFW_REPEAT)) load();
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) { cam.eye.z -= 0.01f; cam.center.z -= 0.01f; }
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) { cam.eye.x -= 0.01f; cam.center.x -= 0.01f; }
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) { cam.eye.z += 0.01f; cam.center.z += 0.01f; }
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) { cam.eye.x += 0.01f; cam.center.x += 0.01f; }

	view_matrix = glm::lookAt(
		cam.eye, // the position of your camera, in world space
		cam.center, // where you want to look at, in world space
		cam.up // probably glm::vec3(0,1,0)
	);
}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
}

int main(void) {
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	if (!glfwInit())return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "OSCARJULY TETERA!", NULL, NULL);
	if (!window) {glfwTerminate();	return -1;}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	Obj tetera;
	tetera.mesh_path = "teapot_small/teapot_small.obj";
	tetera.shader_path = { "src/shader.vert", "src/shader.frag" };
	tetera.model = translate(mat4(.5f), vec3(0.f, 0.f, -1.0f));
	objs.push_back(tetera);

	//load all the resources
	load();

    while (!glfwWindowShouldClose(window))
    {
		draw();
        
        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
        
        //mouse position must be tracked constantly (callbacks do not give accurate delta)
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
    }

    //terminate glfw and exit
    glfwTerminate();
    return 0;
}


