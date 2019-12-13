#version 330

// vertex shader
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 vNormal;

// output values that will be interpolatated per-fragment
out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec3 fS;

smooth out vec2 texture;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 SpotPos;
uniform vec4 LightPosition;

void main()
{

    vec4 v = vec4(vPosition, 1.0f);

    fN = mat3(transpose(inverse(Model))) * vNormal;
    fE = -(Model * v).xyz;
    fL = LightPosition.xyz;
    fS = SpotPos + fE;

    if( LightPosition.w != 0.0 ) {
	    fL = LightPosition.xyz + fE;
    }

    gl_Position = Projection * View * Model * v;

    texture = vTexture;
}
