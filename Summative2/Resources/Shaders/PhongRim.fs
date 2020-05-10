#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex;
uniform sampler2D tex1;

uniform float ambientStr = 0.05f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos = vec3(-2.0f, 6.0f, 3.0f);

uniform vec3 camPos; //= vec3(0.0f, 0.0f, 6.0f);
uniform float lightSpecStr = 1.0f;
uniform float shininess = 32.0f;

uniform int rimExponent = 2;
uniform vec3 rimColor = vec3 (1.0f, 0.0f, 0.0f);

void main ()
{
// Ambient
vec3 ambient = ambientStr * ambientColor;

// Light Direction
vec3 norm = normalize(fragNormal);
vec3 lightDir = normalize(fragPos - lightPos);

// Diffuse Coloring
float diffuseStr = max(dot(norm, -lightDir), 0.0f);
vec3 diffuse = diffuseStr * lightColor;

// Specular Highlight
vec3 negViewDir = normalize((1.0f* camPos) - fragPos);

// Full Phong
//vec3 reflectDir = reflect(lightDir, norm);
//float spec = pow(max(dot(negViewDir, reflectDir), 0.0f), shininess);

// Blinn-Phong
vec3 halfwayVec = normalize(-lightDir + negViewDir);
float spec = pow(max(dot(norm, halfwayVec), 0.0f), shininess);

// Rim Lighting
float rimFactor = 1.0 - dot(norm, negViewDir);
rimFactor = smoothstep(0.0, 1.0, rimFactor);
rimFactor = pow(rimFactor, rimExponent);
vec3 rim = rimFactor * rimColor * lightColor;

vec3 specular = lightSpecStr * spec * lightColor;

color = vec4(ambient + diffuse + specular + rim, 1.0f) * (texture(tex, fragTexCoord));
}