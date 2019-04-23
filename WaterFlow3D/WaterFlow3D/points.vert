#version 430

layout(location = 0) in vec3 position;
layout(location = 0) uniform mat4 mvp;
layout(location = 1) in int type;
layout(location = 2) in float density;
out vec4 colour;
const vec2 r = vec2(cos(0),sin(0));
const vec2 g = vec2(cos(2./3. * 3.14),sin(2./3. * 3.14));
const vec2 b = vec2(cos(4./3. * 3.14),sin(4./3. * 3.14));
void main() {
	float theta = (3.14*4./3.) * max(0.0,min(1.0,density / 4.0));
	vec2 colourpos = vec2(cos(theta),sin(theta));
	colour = vec4(max(0,dot(colourpos,r)),max(0,dot(colourpos,g)),max(0,dot(colourpos,b)),1.);
	colour = vec4(type,1,1,1);
	vec3 size = vec3(100,100,100);
	gl_Position = mvp * vec4(position,1);
	//gl_Position = vec4(((position/size) * 2.0) - 1,1) * mvp; 
	const float max_size = 60000;
	gl_PointSize = max(1,(max_size / (gl_Position.z*gl_Position.z)));
}