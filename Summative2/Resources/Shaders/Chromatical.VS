#version 450 core 

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec2 fragTexCoord;

uniform mat4 translation;
//uniform mat4 rotation;
//uniform mat4 scale;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

void main ()
{
	gl_Position = translation * vec4(position.x, 1 - position.y, 0.0, 1.0);
	// fragColor = color;
	fragTexCoord = texCoord;
}