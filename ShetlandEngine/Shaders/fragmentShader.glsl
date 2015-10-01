#version 430

in vec2 fragTexUV;
uniform vec4 color;
uniform sampler2D uniformTex;

void main()
{
	vec4 color = texture(uniformTex, fragTexUV);
	gl_FragColor = (color);
}