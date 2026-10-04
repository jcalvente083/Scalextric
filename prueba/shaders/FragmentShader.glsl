#version 400

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 ShadowCoord;

uniform sampler2D BaseTex;
uniform sampler2DShadow ShadowMap;
uniform mat4 ViewMatrix;

struct LightInfo {
	vec3 Ldir;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};
uniform LightInfo Light;

struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
	float Dissolved;
};
uniform MaterialInfo Material;

out vec4 FragColor;

vec3 ads(vec3 TexColor)
{
	vec4 s4 = ViewMatrix*vec4(Light.Ldir, 0.0);
	vec3 n = normalize(Normal);
	vec3 v = normalize(-Position);
	vec3 s = normalize(-vec3(s4));
	vec3 r = reflect(-s, n);
	float dRate = max(dot(s, n), 0.0);
	float sRate = pow(max(dot(r, v), 0.0), Material.Shininess);
	vec3 difusse = Light.Ld * Material.Kd * dRate;
	vec3 specular = Light.Ls * Material.Ks * sRate;
	return difusse*TexColor + specular;
}

void main()
{
	vec4 Texel = texture(BaseTex, TexCoord);
	vec3 TexColor = vec3(Texel);
	vec3 ambient = Light.La * Material.Ka * TexColor;
	vec3 diffAndSpec = ads(TexColor);

	// Percentage-closer filtering (PCF): media de cuatro consultas desplazadas
	vec4 ShadowTexCoord = (ShadowCoord/ShadowCoord.w)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
	float sum = 0;
	sum += textureProjOffset(ShadowMap, ShadowTexCoord, ivec2(-1,-1));
	sum += textureProjOffset(ShadowMap, ShadowTexCoord, ivec2(-1,1));
	sum += textureProjOffset(ShadowMap, ShadowTexCoord, ivec2(1,1));
	sum += textureProjOffset(ShadowMap, ShadowTexCoord, ivec2(1,-1));
	float shadow = sum * 0.25;

	// La opacidad combina la del material y la de la textura (cristal del coche)
	FragColor = vec4(shadow * diffAndSpec + ambient, Material.Dissolved * Texel.a);
}
