#version 430
#define SIZEX 120.0
#define SIZEY 120.0
layout(location = 0) in vec2 position;
layout(location = 1) in float density;
out vec4 colour;
const vec2 r = vec2(cos(0),sin(0));
const vec2 g = vec2(cos(2./3. * 3.14),sin(2./3. * 3.14));
const vec2 b = vec2(cos(4./3. * 3.14),sin(4./3. * 3.14));
void main() {
	float theta = (3.14*4./3.) * max(0.0,min(1.0,density / 1.0));
	vec2 colourpos = vec2(cos(theta),sin(theta));
	colour = vec4(max(0,dot(colourpos,r)),max(0,dot(colourpos,g)),max(0,dot(colourpos,b)),1.);
	if(density == -1)
	{
	colour = vec4(0,0,0,1);
	}
	if (density > 0)
	{
		//colour = vec4(1,1,1,1);
	}
//	colour = vec4(density,0,0,1);
	//colour = vec4(0,1,1,1);
	gl_Position = vec4(((position)/vec2((SIZEX - 1.0)/2.0,(SIZEY - 1.0)/2.0)) - vec2(1,1),0,1);
}