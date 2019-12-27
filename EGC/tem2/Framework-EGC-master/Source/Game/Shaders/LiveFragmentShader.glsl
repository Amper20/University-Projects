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
uniform int spot;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	float ambient_light = 0.9;


	vec3 N = normalize(world_normal);
	vec3 Wp = normalize(world_position);
	vec3 L = normalize(light_position - world_position);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(eye_position + Wp);

	float specular_light = pow(max(dot(R, V), 0), material_shininess);

	float diffuse_light = max(dot(N, L), 0);

	float d = distance(light_position, world_position);

	vec3 color = (ambient_light + (specular_light*material_ks + diffuse_light * material_kd)*(1 / (d*d + 1))) * object_color;
	out_color = vec4(min(1.0,color.x), min(1.0, color.y), min(1.0, color.z), 1);
	//out_color = vec4(object_color, 1);
}