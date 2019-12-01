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
	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component


	// TODO: compute light
	vec3 N = normalize(world_normal);
	vec3 Wp = normalize(world_position);
	vec3 L = normalize(light_position - world_position);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(eye_position + Wp);
	
	// TODO: define ambient light component

	// TODO: compute diffuse light component
	float specular_light = pow(max(dot(R, V), 0), material_shininess);

	float diffuse_light = max(dot(N, L), 0);

	if (diffuse_light > 0)
	{
	}

	// TODO: compute light

	// TODO: send color light output to fragment shadersss
	float d = distance(light_position, world_position);
	
	vec3 color = (ambient_light + (specular_light*material_ks + diffuse_light * material_kd)*(1 / (d*d + 1))) * object_color;
	if(spot == 1)
		out_color = vec4(color, 1);
	else {
		float cut_off = radians(30);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off);

		if (spot_light > spot_light_limit){
			// Quadratic attenuation
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			vec3 color = (ambient_light + (specular_light*material_ks + diffuse_light * material_kd)*(light_att_factor)) * object_color;
			out_color = vec4(color, 1);

		}else
			out_color = vec4(ambient_light * object_color,1);
	}
}