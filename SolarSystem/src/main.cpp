/*  by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) */

// OSCAR JULIÁN PONTE - GI - oscar.julian
// ORIOL GUIMO MORELL - GI - oriol.guimo

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
#include "imageloader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

//include library to get miliseconds do the object moves in circle with time
#include <windows.h>

constexpr float ROTATIONSPEED = 0.001;
constexpr float CAMERA_RADIUS = 3.0f;

//global variables to help us do things
int g_ViewportWidth = 720; int g_ViewportHeight = 720;	// Default window size, in pixels
double mouse_x, mouse_y;	//variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f);		// background colour - a GLM 3-component vector

GLuint g_simpleShader = 0; //shader identifier
GLuint g_Vao = 0; //vao
GLuint g_NumTriangles = 0; //  Numbre of triangles we are painting.
GLuint textureid;

struct Mesh {
	GLuint Vao = 0;
	GLuint NumTriangles = 0;
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

GLuint texture;
vec3 front = cam.center - cam.eye;
vec3 normalizefront = glm::normalize(front);
vec3 side = glm::cross(cam.up, normalizefront);
vec3 normalizeside = glm::normalize(side);
vec3 top = glm::cross(normalizeside, normalizefront);
vec3 g_light_dir(100, 100, 100);


// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
int64_t getCurrentTime() {

	SYSTEMTIME time;
	GetSystemTime(&time);
	LONG time_ms = (time.wSecond * 1000) + time.wMilliseconds;
	int64_t currenttime = time_ms;

	return currenttime;
}
int64_t lasttime = getCurrentTime();
float lastAngle = 0;

int selected = -1;
std::map<std::string, Mesh> meshes;
std::map<std::pair<std::string, std::string>, GLuint> shaders;

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load() {
	//**********************
	// CODE TO SET GEOMETRY
	//**********************
	std::string basepath = "assets/";
	std::string inputfile = basepath + "sphere.obj";
	std::vector< tinyobj::shape_t > shapes;
	std::vector< tinyobj::material_t > materials;
	std::string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), basepath.c_str());

	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************

	//load the shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader = simpleShader.program;

	//sky.simpleShader = shaders[sky.shader_path];

	//std::string inputfile = basepath + sky.mesh_path;

	Mesh mesh;

	// Create the VAO where we store all geometry (stored in g_Vao)
	g_Vao = gl_createAndBindVAO();

	//create vertex buffer for positions, colors, and indices, and bind them to shader
	gl_createAndBindAttribute(&(shapes[0].mesh.positions[0]), shapes[0].mesh.positions.size() * sizeof(float), g_simpleShader, "a_vertex", 3);
	gl_createAndBindAttribute(&(shapes[0].mesh.normals[0]), shapes[0].mesh.normals.size() * sizeof(float), g_simpleShader, "a_normal", 3);
	gl_createAndBindAttribute(&(shapes[0].mesh.texcoords[0]), shapes[0].mesh.texcoords.size() * sizeof(float), g_simpleShader, "a_uv", 2);
	gl_createIndexBuffer(&(shapes[0].mesh.indices[0]), shapes[0].mesh.indices.size() * sizeof(unsigned int));
	//gl_createAndBindAttribute(shapes[0].mesh.texcoords.data(), shapes[0].mesh.texcoords.size() * sizeof(float), o.simpleShader, "a_uv", 2);

	Image* image = loadBMP("assets/earthmap1k.bmp");

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_simpleShader, "u_texture");

	//bind the sampler to teture unit 0
	glUniform1i(u_texture, 0);

	g_NumTriangles = shapes[0].mesh.indices.size() / 3;

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glGenTextures(1, &textureid);

	glBindTexture(GL_TEXTURE_2D, textureid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	//unbind everything
	gl_unbindVAO();
}

// ------------------------------------------------------------------------------------------
// This function actulally draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw() {
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.2, .2, .2, .0);
	glCullFace(GL_CW);

	mat4 projection_matrix = perspective(
		60.0f, // Field of view
		1.0f,  // Aspect ratio
		0.1f,  // near plane (distance from camera) 
		50.0f  // Far plane (distance from camera)
	);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	mat4 model_matrix = translate(mat4(1.0f), vec3(cam.center));   //translate identity in -0.5x -0.6y 
	mat4 view_matrix = glm::lookAt(cam.eye, cam.center, cam.up);

	GLuint view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// activate shader
	glUseProgram(g_simpleShader);

	//bind the geometry
	gl_bindVAO(g_Vao);

	GLuint colorLoc = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc, 0.0, 0.0, 1.0);

	GLuint u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	GLuint model_loc = glGetUniformLocation(g_simpleShader, "u_model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

	GLuint u_eye = glGetUniformLocation(g_simpleShader, "u_eye");
	glUniform3f(u_eye, cam.eye.x, cam.eye.y, cam.eye.z);

	//find sampler uniform in shader
	int u_texture = glGetUniformLocation(g_simpleShader, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	int64_t currenttime = getCurrentTime();
	int64_t delta = currenttime - lasttime;
	lasttime = currenttime;
	lastAngle += delta * ROTATIONSPEED;

	cam.eye.x = CAMERA_RADIUS * cos(lastAngle) + 0.0;
	cam.eye.y = CAMERA_RADIUS * sin(lastAngle) + 0.0;

	cam.up = vec3(0.0, 0.0, 0.0) - cam.eye;
	cam.up.z = 0.0;

	//o.color.r = sin(glfwGetTime()) * .5 + .5;
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
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "OSCAR_JULIAN & ORIOL GUIMO", NULL, NULL);
	if (!window) { glfwTerminate();	return -1; }
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//sky.mesh_path = "teapot_small/sphere.obj";
	//sky.shader_path = { "src/shader2.vert", "src/shader2.frag" };
	//sky.color = vec4(1., 0., 0., 1.);
	//objs.push_back(sky);

	//load all the resources
	load();

    while (!glfwWindowShouldClose(window))
    {
		//sky.model = translate(mat4(.5f), cam.eye);
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
