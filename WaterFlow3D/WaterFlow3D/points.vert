#version 430

layout(location = 0) in vec3 position;
//layout(location = 1) in float type;
out vec4 colour;
void main() {
	int type = 1;	
	/*if(type >= 0.5)
	{
		colour = vec4(1,1,1,1);	
	}
	else
	{
		colour = vec4(0,0,1,1);
	}*/
	colour = vec4(type,1,1,1);
	vec3 size = vec3(100,100,100);
	gl_Position = vec4(((position/size) * 2.0) - 1,1); 
	const float max_size = 10;
	gl_PointSize = max_size * -gl_Position.z;//max(1,int(max_size / max(position.z,1)));
}