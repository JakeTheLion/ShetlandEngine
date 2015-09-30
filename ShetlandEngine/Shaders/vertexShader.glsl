#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vecTexUV;
out vec2 fragTexUV;
uniform mat4 worldMatrix;

void main()
{
	fragTexUV = vecTexUV;
	gl_Position = worldMatrix * vec4(position, 1);
}