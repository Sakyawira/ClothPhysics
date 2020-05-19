#version 450 core 

in vec3 outColor; 

in vec2 TexCoords;

uniform sampler2D tex;

out vec4 color;

void main()
{
    vec4 texColor = texture(tex, vec2(TexCoords.x , TexCoords.y));
    if(texColor.a < 0.1)
    {
        discard;
    }
   
    color = texColor;// * vec4(123.0f/255.0f, 173.0f/255.0f, 203.0f/255.0f, 1.0f); 
}