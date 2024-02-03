//------------------------------------------------------------------------------------------------------------
//ï˙éÀèÛÉuÉâÅ[
//------------------------------------------------------------------------------------------------------------
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR0;
};

cbuffer psConstants1 : register(b1)
{
	float g_power;
	float2 g_texSize;
};

//float Gauss(float x, float y)
//{
//	float stDev = 16;
//	float pi = 3.14159265359;
//	float v = stDev*stDev;
//	return 	exp((x*x + y*y) / (-2 * v)) / (sqrt(2 * pi*v));
//}

float4 PS(VS_OUTPUT input) : SV_Target
{

	const float2 uv = input.tex;

	float4 outPut = float4(0, 0, 0,0);
	//float weight[3][3] =
	//{
	//	{1 / 16.0,1 / 8.0,1 / 16.0},
	//	{ 1 / 8.0,1 / 4.0,1 / 8.0 },
	//	{ 1 / 16.0,1 / 8.0,1 / 16.0 },
	//};
	static float weight[5][5] =
	{
		{ 1,4,6,4,1},
		{ 4,16,24,16,4 },
		{ 6,24,36,24,6 },
		{ 4,16,24,16,4 },
		{ 1,4,6,4,1 }
	};

	const float p = g_power;
	for (int x = -2*p; x <= 2*p; x++)
		for (int y = -2*p; y <= 2*p; y++)
		{
			float2 tmp = uv - float2(x / g_texSize.x, y / g_texSize.y);
			tmp.x = min(1, max(0, tmp.x));
			tmp.y = min(1, max(0, tmp.y));
			outPut += texture0.Sample(sampler0, tmp)*weight[x/p + 2][y/p + 2]/(256.0*p*p/1.1);
		}
	return outPut*input.color;
}
