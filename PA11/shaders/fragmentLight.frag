#version 330

// fragment shader

// per-fragment interpolated values from the vertex shader
in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec3 fS;

smooth in vec2 texture;

out vec4 frag_color;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

uniform vec3 DiffuseColor;

uniform vec3 SpotPos;
uniform vec3 SpotDir;
uniform float SpotCutOff;

uniform bool PointLightEnabled;
uniform vec4 NightAmbient;

uniform sampler2D sampler;

vec4 calcLight(vec3 N, vec3 E, vec3 L, vec3 dColor, float dist);

void main()
{
    // Normalize the input lighting vectors

    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    vec3 S = normalize(fS);

    vec4 light_color;

    if(PointLightEnabled)
    {
        light_color = AmbientProduct + calcLight(N, E, L, DiffuseColor, 0);
    } else
    {
        light_color = NightAmbient;
    }

    // calculate spotlight
    float spotTheta = dot(S, normalize(-SpotDir));

    if(spotTheta > SpotCutOff)
    {
        // do lighting calculations
        light_color += calcLight(N, E, S, vec3(30, 30, 33), length(fS));
    } else
    {
        //light_color = vec4(0, 0, 1, 1);
    }

    light_color.a = 1.0;

    vec4 text_color = texture2D(sampler, texture);
    frag_color = light_color * text_color;
}

vec4 calcLight(vec3 N, vec3 E, vec3 L, vec3 dColor, float dist)
{
    vec3 H = normalize( L + E );

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseProduct*vec4(dColor, 1);

    if(dist != 0)
    {
        diffuse = pow(1.0 / (dist + 1), 2) * diffuse;
    }

    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 )
	specular = vec4(0.0, 0.0, 0.0, 1.0);

    if(dist != 0)
    {
        specular = pow(1.0 / (dist + 1), 2) * specular;
    }

    return diffuse + specular;
}
