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
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

void main()
{
    vec4 v = vec4(vPosition, 1.0);
    // Transform vertex  position into eye coordinates
    vec3 pos = (ModelView * v).xyz;
	
    vec3 L = normalize( LightPosition.xyz - pos );
    vec3 E = normalize( -pos );
    vec3 H = normalize( L + E );

    // Transform vertex normal into eye coordinates
    vec3 N = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;
    // Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;

    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;
    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;
    if( dot(L, N) < 0.0 )
    {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
    gl_Position = Projection * ModelView * v;

    color = ambient + diffuse + specular;
    color.a = 1.0;

    //color = vec4(0, 0, 0, 1);

    texture = vTexture;
}