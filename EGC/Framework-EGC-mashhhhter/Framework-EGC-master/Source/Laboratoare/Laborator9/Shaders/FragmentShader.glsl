#version 330

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float time;
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	
	vec4 color1 = texture2D(texture_1, vec2(texcoord.x + sin(time) , texcoord.y + cos(time)));
	if (color1.a < 0.5)
		discard;
	vec4 color2 = texture2D(texture_2, vec2(texcoord.x + sin(time) , texcoord.y + cos(time)));
	if (color2.a < 0.5)
		discard;
	//vec3 color = 
	out_color = mix(color1, color2, 0.5f);
}