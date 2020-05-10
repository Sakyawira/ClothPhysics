#version 450 core

in  vec3  position;

out vec4 mWorldPos;

void main()
{
	gl_Position = vec4(position, 1.0f);
}