# version 330 core

layout (location = 0) in vec3 _vertex;
layout (location = 1) in vec2 _texcoord0;

out vec2 texcoord0;

void main (void)
{
	texcoord0 = _texcoord0;
    gl_Position = vec4 (_vertex.xy, 0, 1);
}

