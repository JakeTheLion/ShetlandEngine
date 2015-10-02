#version 430

layout (location = 0) in vec3 vecPos;
layout (location = 1) in vec2 vecTexUV;
layout (location = 2) in vec3 vecNorm;
out vec2 fragTexUV;
out vec3 fragPos;
out vec3 fragNorm;
uniform mat4 worldMatrix;
uniform mat4 cameraMatrix;

void main()
{
	gl_Position = cameraMatrix * worldMatrix * vec4(vecPos, 1);
	fragPos = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
	fragNorm = normalize(vecNorm);
	fragTexUV = vecTexUV;
}