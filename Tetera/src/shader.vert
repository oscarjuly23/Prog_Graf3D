#version 330
 
in vec3 a_vertex;
in vec3 a_color;
in vec3 a_normal;

out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;


void main()
{
	// position of the vertex
	vec3 v_pos = (u_model * vec4(a_vertex,1.0)).xyz;
	gl_Position = u_projection * u_view * u_model * vec4(v_pos, 1.0 );

	v_normal = -a_normal;
	
	// pass the colour to the fragment shader
	//v_color = a_color;
}

