#version 330

in vec3 v_color;
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_pos;
out vec4 fragColor;

uniform vec3 u_color;
uniform sampler2D u_texture;
uniform vec3 u_light_dir;
uniform vec3 u_light;
uniform vec3 u_light_color;
uniform vec3 u_eye;


void main(void)
{
	// We're just going to paint the interpolated colour from the vertex shader
	vec3 texture_color = texture(u_texture, v_uv).xyz;
	vec3 ambient_color = texture_color * 0.1f;

	vec3 N=normalize(v_normal);
	vec3 L=normalize (u_light_dir);
	 float NdotL = max(dot (N,L), 0.0);

	 vec3 diffuse_color = texture_color * NdotL;
	vec3 R=reflect(-L,N);
	vec3 E=normalize (u_eye-v_pos);

	float RdotE = max(0.0,dot(R,E));
	vec3 specular_color = vec3(1.0,1.0,1.0)*pow(RdotE,50);

	fragColor = vec4(ambient_color + diffuse_color + specular_color,1.0);
}