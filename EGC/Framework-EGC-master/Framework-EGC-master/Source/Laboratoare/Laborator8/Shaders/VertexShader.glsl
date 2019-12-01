#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec3 L;
out vec3 R;
out vec3 V;

void main()
{
	// TODO: compute world space vertex position and normal
	world_position = vec3(Model*vec4(v_position, 1.0));
	world_normal = normalize(mat3(Model)*v_normal);

	L = normalize(eye_position - world_pos);
	R = reflect(-L, N);
	V = normalize(Po + Pv);

	// TODO: send world position and world normal to Fragment Shader
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
