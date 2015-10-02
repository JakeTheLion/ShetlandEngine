#version 430

in vec2 fragTexUV;
in vec3 fragPos;
in vec3 fragNorm;
uniform sampler2D textureSampler;

void main()
{
	vec4 texColor = texture(textureSampler, fragTexUV);

	vec3 lightDir = vec3(0, 1, 0);
	float lamb = max(dot(lightDir, fragNorm), 0);
	vec3 viewDir = normalize(-fragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spcAngle = max(dot(halfDir, fragNorm), 0);
	float specular = pow(spcAngle, 16.0);
	float light = 0.225 + 0.525 * lamb + 0.25 * specular;

	gl_FragColor = vec4(texColor.rgb * light, 1);
}