# version 330 core

in vec4 vertex;
in vec3 normal;

out vec4 color;

const int MAX_NUM_LIGHTS = 8;
uniform bool enablelighting;
uniform int num_lights;

uniform vec4 ambient_color;
uniform vec4 diffuse_color;
uniform vec4 specular_color;
uniform vec4 emission_color;
uniform float shininess;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewMatrixInverseTranspose;

struct Light {
    vec4 position;
    vec4 color;
};

uniform Light lights[MAX_NUM_LIGHTS];

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec) {

    float nDotL = dot (normal, direction);
    vec4 lambert = diffuse_color * lightcolor * max (nDotL, 0.0);

    float nDotH = dot (normal, halfvec);
    vec4 phong = specular_color * lightcolor * pow (max (nDotH, 0.0), shininess);

    vec4 retval = lambert + phong;
    return retval;
}

void main (void)
{
    if (enablelighting) {
        const vec3 eyepos = vec3 (0, 0, 0);
        vec4 tmppos = ModelViewMatrix * vertex;
        vec3 pos = tmppos.xyz / tmppos.w; // Dehomogenize current location
        vec3 eyedirn = normalize(eyepos - pos);

        // Compute normal, needed for shading.
        vec4 tmpnormal = ModelViewMatrixInverseTranspose * vec4 (normal,0);
        vec3 inormal = normalize (tmpnormal.xyz);

        vec4 fragColor = ambient_color + emission_color;
        for (int i=0; i<num_lights; ++i)
        {
            // Directional
            if (lights[i].position.w == 0) {
                vec3 ldir = normalize (lights[i].position.xyz);
                vec3 h = normalize (ldir + eyedirn);
                fragColor += ComputeLight (ldir, lights[i].color, inormal, h);
            }
            // Point
            else {
                vec3 lpos = lights[i].position.xyz / lights[i].position.w;
                vec3 ldir = normalize (lpos - pos); // no attenuation
                vec3 h    = normalize (ldir + eyedirn);
                fragColor += ComputeLight (ldir, lights[i].color, inormal, h);
            }
        }
        color = fragColor;

    } else {
        color = diffuse_color;
    }
}
