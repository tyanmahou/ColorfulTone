
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR0;
};


float4 PS(VS_OUTPUT input) : SV_Target
{
	float2 uv = input.tex;
	float4 srcColor = texture0.Sample(sampler0, uv);
	float alpha;
	if (uv.y < 0.5)
	{
		alpha = 1.0 - uv.y;

	}
	else if (uv.y>0.5)
	{
		alpha = uv.y;
	}
	else
		alpha = 0;

	srcColor.a =2.0*pow(alpha,6);

	return srcColor*input.color;
}
