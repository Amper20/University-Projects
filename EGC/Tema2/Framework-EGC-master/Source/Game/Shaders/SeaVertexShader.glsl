#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
uniform vec3 randm;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;


void main()
{
	world_normal = normalize(mat3(Model)*v_normal);
	float y = (sin(time) + cos(time))/13;
	
	vec3 newPosition = vec3(v_position.x, v_position.y, v_position.z);

	newPosition.z += sin(v_position.x * 10) * y;
	newPosition.x += sin(v_position.y * 10) * y;
	// move the position along the normal and transform 
	
	world_position = vec3(Model*vec4(newPosition, 1.0));

	// TODO: send world position and world normal to Fragment Shader
	gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
}
