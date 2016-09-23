# version 130

in vec2 texcoord0;

uniform sampler2D texture0;

void main (void)
{
    gl_FragColor = texture (texture0, texcoord0);
}
