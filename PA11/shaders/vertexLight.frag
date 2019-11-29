#version 330

// fragment shader

smooth in vec4 color;
smooth in vec2 texture;

out vec4 frag_color;

uniform sampler2D sampler;

void main() 
{
    vec4 text_color = texture2D(sampler, texture);
    frag_color = color * text_color;
}