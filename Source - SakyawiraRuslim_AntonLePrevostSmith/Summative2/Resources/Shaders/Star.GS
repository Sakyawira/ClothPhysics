#version 450 core

layout (points) in; 

layout (triangle_strip, max_vertices = 9) out;

out vec2 TexCoords; 

out vec3 outColor;

in VS_GS_VERTEX{ 
	in vec4 position; 
	in vec3 color; 
	in mat4 mvp; 
	in vec2 texCoords;
}
gs_in[];

void main() 
{
	outColor = gs_in[0].color;

	// Left Triangle
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(1.3f, -1.95f, 0.0f, 0.0f);  TexCoords = vec2(0.0f, 0.0f); EmitVertex();
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, -1.17f, 0.0f, 0.0f);  TexCoords = vec2(1.0f, 0.0f); EmitVertex();
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, 1.3f, 0.0f, 0.0f);    TexCoords = vec2(0.5f, 1.0f); EmitVertex();
	EndPrimitive();

	// Right Triangle
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, 1.3f, 0.0f, 0.0f);	  TexCoords = vec2(0.0f, 0.0f);EmitVertex();
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, -1.17f, 0.0f, 0.0f);  TexCoords = vec2(1.0f, 0.0f);EmitVertex();	
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-1.3f, -1.95f, 0.0f, 0.0f); TexCoords = vec2(0.5f, 1.0f);EmitVertex();
	EndPrimitive();	

	// Middle Triangle
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, -1.17f, 0.0f, 0.0f); TexCoords = vec2(0.0f, 0.0f);EmitVertex();
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-1.95f, 0.26f, 0.0f, 0.0f);TexCoords = vec2(1.0f, 0.0f);EmitVertex();
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(1.95f, 0.26f, 0.0f, 0.0f); TexCoords = vec2(0.5f, 1.0f);EmitVertex();
	EndPrimitive();	
}