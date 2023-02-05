struct VS_INPUT
{
	float4 position:  POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT
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
	row_major float4x4 m_projection;
	float4 m_light_direction;
	float4 m_cam_pos;

};


VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, m_world);
	output.direction_cam = normalize(output.position.xyz - m_cam_pos.xyz);

	output.position = mul(output.position, m_view);

	output.position = mul(output.position, m_projection);


	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}
