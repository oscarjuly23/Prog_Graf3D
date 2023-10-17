#version 330

out vec4 fragColor;

in vec3 v_color;
uniform vec4 u_color;

void main(void)
{
	// We're just going to paint the interpolated colour from the vertex shader
	fragColor =  u_color;
}