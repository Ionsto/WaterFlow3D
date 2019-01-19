#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in float type;
out vec4 colour;
void main() {
	/*if(type >= 0.5)
	{
		colour = vec4(1,1,1,1);	
	}
	else
	{
		colour = vec4(0,0,1,1);
	}*/
	colour = vec4(type,1,1,1);
	gl_Position = vec4((position * 2.0) - 1,1); 
}