#version 330

// TODO: get values from fragment shader
in vec3 frag_normal;
in vec3 frag_color;
in vec3 frag_position;

out vec4 out_position;
out vec4 out_normal;
out vec4 out_color;

void main()
{
	// TODO: write pixel out color
	out_color = vec4(frag_normal, 1);
	out_normal = vec4(frag_normal, 1);
	out_position = vec4(frag_position, 1);
}