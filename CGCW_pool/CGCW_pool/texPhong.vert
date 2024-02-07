#version 450 core

layout(location = 0) in vec4 vPos;
layout(location = 1) in vec2 vTex;
layout(location = 2) in vec3 vNor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
flat out vec3 nor;
out vec3 FragPosWorldSpace;

void main()
{
	gl_Position = projection * view * model * vPos;
	texCoord = vTex;
	nor = mat3(inverse(transpose(model))) * vNor;
	//nor = vNor;
	FragPosWorldSpace = vec3(model * vPos);
}
