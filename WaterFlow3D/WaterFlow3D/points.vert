#version 150

in vec4 position;

void main() {
	gl_Position = (position * vec4(2,2,2,1)) - vec4(1,1,1,0);// - vec4(1,1,0,0);
}