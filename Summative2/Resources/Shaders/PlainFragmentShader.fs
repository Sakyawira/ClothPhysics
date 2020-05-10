#version 330 core

in vec3 fragColor;

out vec4 color;

uniform sampler2D tex;

void main() {
    color = vec4(fragColor, 1.0f);
}