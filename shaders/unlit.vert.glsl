# version 130

attribute vec3 position;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

void main (void)
{
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4 (position, 1);
}

