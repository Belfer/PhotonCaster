# version 120

attribute vec3 _vertex;
attribute vec2 _texcoord0;

varying vec2 texcoord0;

void main (void)
{
	texcoord0 = _texcoord0;
    gl_Position = vec4 (_vertex.xy, 0, 1);
}

