# version 130

attribute vec3 _vertex;
attribute vec3 _normal;

out vec4 vertex;
out vec3 normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

void main (void)
{
    vertex = vec4 (_vertex, 1);
    normal = _normal;
    gl_Position = ProjectionMatrix * ModelViewMatrix * vertex;
}

