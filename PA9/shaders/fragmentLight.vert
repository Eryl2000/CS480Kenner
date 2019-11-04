#version 330

// vertex shader 
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 vNormal;

// output values that will be interpolatated per-fragment
smooth out vec3 fN;
smooth out vec3 fE;
smooth out vec3 fL;

smooth out vec2 texture;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
    vec4 v = vec4(vPosition, 1.0);
    vec3 pos = (ModelView * v).xyz;
    fN = (ModelView*vec4(vNormal, 0.0)).xyz;
    fE = -pos;
    fL = LightPosition.xyz - pos;
    
    if( LightPosition.w != 0.0 ) {
	    fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = Projection*ModelView*v;

    texture = vTexture;
}