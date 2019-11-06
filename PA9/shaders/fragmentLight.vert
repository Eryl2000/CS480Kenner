#version 330

// vertex shader 
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 vNormal;

// output values that will be interpolatated per-fragment
smooth out vec3 fN;
smooth out vec3 fE;
out vec3 fL;

smooth out vec2 texture;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec4 LightPosition;

void main()
{

    vec4 v = vec4(vPosition, 1.0f);

    fN = (Model * vec4(vNormal, 0.0)).xyz;
    fE = (Model * v).xyz;
    fL = LightPosition.xyz;
    
    if( LightPosition.w != 0.0 ) {
	    fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = Projection * View * Model * v;

    texture = vTexture;
}