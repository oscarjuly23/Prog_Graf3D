#include "glfunctions.h"

GLuint gl_createAndBindVAO() {
	GLuint new_vao;
#ifdef __APPLE__
	glGenVertexArraysAPPLE(1, &new_vao); // glGenVertexArrays requires a reference
	glBindVertexArrayAPPLE(new_vao); //bind it now so we can start writing to it
#else
	glGenVertexArrays(1, &new_vao); // glGenVertexArrays requires a reference
	glBindVertexArray(new_vao); //bind it now so we can start writing to it
#endif
	return new_vao;
}

void gl_createAndBindAttribute(const GLfloat data[], int data_size, GLuint shader, const char* attrib, GLuint attrib_size) {
	//int to store created buffers
	GLuint buffer;
	// Crea VBO per vèrtexs
	glGenBuffers(1, &buffer); //glGenBuffers requires a reference
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //bind our new buffer
	glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW); //fill it with out vertices

   // Activate shader attribute for this buffer
	GLuint vertexLoc = glGetAttribLocation(shader, attrib); //find attribute vertexPos in shader
	glEnableVertexAttribArray(vertexLoc); //enable it
	glVertexAttribPointer(vertexLoc, attrib_size, GL_FLOAT, GL_FALSE, 0, 0); //specify location and data format

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffers
}

void gl_createIndexBuffer(const GLuint* data, int data_size) {
	GLuint buffer;
	// Create VBO por indices
	glGenBuffers(1, &buffer); //create more new buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); //bind them as element array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW); //fill buffer with index data
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffers
}

void gl_unbindVAO() {
#ifdef __APPLE__
	glBindVertexArrayAPPLE(0); //unbind VAO
#else
	glBindVertexArray(0); //unbind VAO
#endif
}

void gl_bindVAO(GLuint vao) {
#ifdef __APPLE__
	glBindVertexArrayAPPLE(vao);
#else
	glBindVertexArray(vao);
#endif
}