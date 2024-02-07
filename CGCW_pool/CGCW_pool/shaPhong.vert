#version 450 core

layout(location = 0) in vec4 vPos;
// layout(location = 1) in vec3 vCol;
layout(location = 2) in vec3 vNor;

uniform vec3 uCol;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection; 
uniform mat4 projectedLightSpaceMatrix;

out vec3 col;
out vec3 nor;
out vec3 FragPosWorldSpace;
out vec4 FragPosProjectedLightSpace;

void main()
{
	gl_Position = projection * view * model * vPos;
	col = uCol;
	nor = mat3(transpose(inverse(model))) * vNor;
	FragPosWorldSpace = vec3(model * vPos);
	FragPosProjectedLightSpace = projectedLightSpaceMatrix * model * vPos;
}
