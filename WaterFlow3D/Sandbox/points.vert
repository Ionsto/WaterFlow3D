#version 430
#define SIZEX 20.0
#define SIZEY 20.0
layout(location = 0) in vec2 position;
layout(location = 1) in float density;
smooth out vec4 colour;
const vec2 r = vec2(cos(0),sin(0));
const vec2 g = vec2(cos(2./3. * 3.14),sin(2./3. * 3.14));
const vec2 b = vec2(cos(4./3. * 3.14),sin(4./3. * 3.14));
void main() {
	float theta = (3.14*4./3.) * max(0.0,min(1.0,density));
	vec2 colourpos = vec2(cos(theta),sin(theta));
	//colour = vec4(1,1,1,1);
	colour = vec4(max(0,dot(colourpos,r)),max(0,dot(colourpos,g)),max(0,dot(colourpos,b)),1.);
	gl_Position = vec4((position/vec2((SIZEX - 1.0)/2.0,(SIZEY - 1.0)/2.0)) - vec2(1,1),0,1);
	gl_PointSize = 1;//max(1,(max_size / (gl_Position.z*gl_Position.z)));
}