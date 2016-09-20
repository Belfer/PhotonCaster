# version 130

in vec2 texcoord0;

uniform sampler2D texture0;

void main (void)
{
    //gl_FragColor = (vec4 (texcoord0, 1, 1) - texture2D (texture0, texcoord0)) * textureSize (texture0, 0).x;
	gl_FragColor = texture (texture0, texcoord0);
}
