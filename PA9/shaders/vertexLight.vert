#version 330

// vertex shader
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 vNormal;

smooth out vec4 color;  //vertex shade
smooth out vec2 texture;

// light and material properties
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

uniform vec3 SpotPos;
uniform vec3 SpotDir;
uniform float SpotCutOff;

vec4 calcLight(vec3 N, vec3 E, vec3 L);

void main()
{
    vec4 v = vec4(vPosition, 1.0);
    // Transform vertex  position into eye coordinates
    vec3 pos = (Model * v).xyz;
	
    vec3 L = normalize( LightPosition.xyz - pos );
    vec3 E = normalize( -pos );

    // Transform vertex normal into eye coordinates
    vec3 N = normalize( Model*vec4(vNormal, 0.0) ).xyz;

    color = AmbientProduct + calcLight(N, E, L);

     // calculate spotlight
    vec3 spotLightDir = normalize(SpotPos + E);
    float spotTheta = dot(spotLightDir, normalize(-SpotDir));
        
    if(spotTheta > SpotCutOff) 
    {       
        // do lighting calculations
        color += calcLight(N, E, spotLightDir);
    }

    color.a = 1.0;

    gl_Position = Projection * View * Model * v;

    texture = vTexture;
}

vec4 calcLight(vec3 N, vec3 E, vec3 L)
{
    vec3 H = normalize( L + E );   
    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseProduct;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) 
	specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    return diffuse + specular;
}