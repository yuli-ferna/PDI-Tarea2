#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

out vec3 fragPos;
out vec2 fragUv;

void main()
{
	
	fragPos = pos;
	fragUv = uv;

	gl_Position = vec4(fragPos, 1);

}