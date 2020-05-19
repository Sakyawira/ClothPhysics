#version 450 core 

layout (points) in; 

layout (triangle_strip, max_vertices = 4) out;

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

	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-2.0, -2.0, 0.0, 0.0); // Bottom Left
	TexCoords = vec2(0.0f, 1.0f);
	EmitVertex(); 
	
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(2.0, -2.0, 0.0, 0.0); // Bottom Right
	TexCoords = vec2(1.0f, 1.0f);
	EmitVertex(); 
	
	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-2.0,  2.0, 0.0, 0.0); // Top Left
	TexCoords = vec2(0.0f, 0.0f);
	EmitVertex();

	gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(2.0,  2.0, 0.0, 0.0);  // Top Right
	TexCoords = vec2(1.0f, 0.0f);
	EmitVertex();

	EndPrimitive();
}