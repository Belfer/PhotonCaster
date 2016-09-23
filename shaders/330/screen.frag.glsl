# version 330 core

in vec2 texcoord0;

out vec4 color;

uniform sampler2D texture0;

void main (void)
{
    color = texture (texture0, texcoord0);
}
