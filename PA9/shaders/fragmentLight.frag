#version 330

// fragment shader

// per-fragment interpolated values from the vertex shader
smooth in vec3 fN;
in vec3 fL;
smooth in vec3 fE;

smooth in vec2 texture;

out vec4 frag_color;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

uniform vec3 SpotPos;
uniform vec3 SpotDir;
uniform float SpotCutOff;

uniform sampler2D sampler;

vec4 calcLight(vec3 N, vec3 E, vec3 L);

void main() 
{ 
    // Normalize the input lighting vectors
    
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);

    vec4 light_color = calcLight(N, E, L);

    // calculate spotlight
    vec3 spotLightDir = normalize(SpotPos + E);
    float spotTheta = dot(spotLightDir, normalize(-SpotDir));
        
    if(spotTheta > SpotCutOff) 
    {       
        // do lighting calculations
        light_color += calcLight(N, E, spotLightDir);
    }

    light_color.a = 1.0;

    vec4 text_color = texture2D(sampler, texture);
    frag_color = light_color * text_color;
} 

vec4 calcLight(vec3 N, vec3 E, vec3 L)
{
    vec3 H = normalize( L + E );   
    vec4 ambient = AmbientProduct;
    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseProduct;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) 
	specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    return ambient + diffuse + specular;
}