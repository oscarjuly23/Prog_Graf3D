#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

GLuint gl_createAndBindVAO();
void gl_createAndBindAttribute(const GLfloat data[], int data_size, GLuint shader, const char* attrib, GLuint attrib_size);
void gl_createIndexBuffer(const GLuint* data, int data_size);
void gl_unbindVAO();
void gl_bindVAO(GLuint vao);