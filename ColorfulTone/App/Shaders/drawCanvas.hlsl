Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR0;
};
cbuffer psConstants1 : register(b1)
{
	float g_timer;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
	const float2 uv = input.tex;
	float4 outPut = texture0.Sample(sampler0,uv);
	float4 mask = texture1.Sample(sampler0,uv);
	float timer = min(1.0,g_timer);
	if(mask.r <= timer)
	{
		outPut.a = 1.0-mask.b;
	} else {
	 	outPut.a = 0;
	}
	return outPut*input.color;
}