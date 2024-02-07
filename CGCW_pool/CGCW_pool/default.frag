#version 450 core 

layout (location = 0) out vec4 fColor;

in vec3 col;

uniform vec3 uCol;

void main() {
	fColor = vec4(uCol, 0.1f);
}