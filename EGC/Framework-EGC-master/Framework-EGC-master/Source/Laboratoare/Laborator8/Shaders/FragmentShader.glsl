#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: define ambient light component
	float ambient_light = 0.25;	
	float spspecular_light = pow(max(dot(R, V), 0), material_shininess);
	float diffuse_light = max(dot(N, L), 0);

	floar d = distance(Pl, Pv);
	out_color = (ambient + (spspecular_light*material_ks + diffuse_light * material_kd)*(1 / (d*d + 1))) * object_color;
}