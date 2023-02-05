Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: TEXCOORD1;
	float3 direction_cam: TEXCOORD2;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_direction;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float ka = 0.1;
	float3 ia = float3(1.0, 1.0, 1.0);
	float3 a_light = ka * ia;
	float kd = 0.7;
	float3 id = float3(1.0, 1.0, 1.0);
	float d_amount = max(0.0, dot(m_light_direction.xyz, input.normal));
	float3 d_light = kd * d_amount * id;
	float ks = 1.0;
	float3 is = float3(1.0, 1.0, 1.0);
	float3 reflected = reflect(m_light_direction.xyz, input.normal);
	float shine = 30.0;
	float s_amount = pow(max(0.0, dot(reflected, input.direction_cam)), shine);
	float3 s_light = ks * s_amount * is;
	float3 final = a_light + d_light + s_light;
	return float4(final, 1.0);
}