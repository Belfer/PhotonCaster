# version 120

varying vec2 texcoord0;

uniform sampler2D texture0;

void main (void)
{
    gl_FragColor = texture2D (texture0, texcoord0);
}
