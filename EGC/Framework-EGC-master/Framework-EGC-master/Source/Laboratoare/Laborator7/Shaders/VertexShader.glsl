#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_position = vec3(Model*vec4(v_position, 1.0));
	vec3 world_normal = normalize(mat3(Model)*v_normal);

	vec3 L = normalize(light_position - world_position);
	vec3 R = reflect(-L, world_normal);
	vec3 V = normalize(eye_position + world_position);
	vec3 N = world_normal;
	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float spspecular_light = pow(max(dot(R, V), 0), material_shininess);

	float diffuse_light = max(dot(N, L), 0);

	if (diffuse_light > 0)
	{
	}

	// TODO: compute light

	// TODO: send color light output to fragment shader
	float ambient = 0.5;
	float d = distance(light_position, world_position);
	color = (ambient + (spspecular_light*material_ks + diffuse_light * material_kd)*(1 / (d*d + 1))) * object_color;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
