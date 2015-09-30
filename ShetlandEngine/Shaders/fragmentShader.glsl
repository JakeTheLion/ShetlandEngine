#version 430

uniform sample2D uniformTex;
in vec2 fragTexUV;

void main()
{
	vec4 color = texture(uniformTex, fragTexUV);
	gl_FragColor = (color * light, 1);
}