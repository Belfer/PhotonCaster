# version 120

attribute vec3 _vertex;
attribute vec3 _normal;

varying vec4 vertex;
varying vec3 normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

void main (void)
{
    vertex = vec4 (_vertex, 1);
    normal = _normal;
    gl_Position = ProjectionMatrix * ModelViewMatrix * vertex;
}

