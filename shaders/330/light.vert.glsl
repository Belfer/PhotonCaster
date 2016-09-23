# version 330 core

layout (location = 0) in vec3 _vertex;
layout (location = 1) in vec3 _normal;

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

