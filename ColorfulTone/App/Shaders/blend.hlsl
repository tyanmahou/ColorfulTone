Texture2D		g_texture0 : register(t0);
SamplerState	g_sampler0 : register(s0);

cbuffer PSConstants2D : register(b0)
{
    float4 g_colorAdd;
    float4 g_sdfParam;
    float4 g_sdfOutlineColor;
    float4 g_sdfShadowColor;
    float4 g_internal;
}

struct PSInput
{
    float4 position	: SV_POSITION;
    float4 color	: COLOR0;
    float2 uv		: TEXCOORD0;
};

cbuffer ShaderParam : register(b1)
{
    float4 g_color;
}

float softlight(float dest, float src)
{
    return src < 0.5 ? 2.0 * dest * src + dest * dest * (1.0 - 2.0 * src) : 2.0 * dest * (1 - src) + sqrt(dest) * (2 * src - 1.0);
}
float4 softlight(float4 dest, float4 src)
{
    float4 color;
    color.r = softlight(dest.r, src.r);
    color.g = softlight(dest.g, src.g);
    color.b = softlight(dest.b, src.b);
    color.a = 1.0;
    return color;
}
float4 PS(PSInput input) : SV_TARGET
{
    const float2 uv = input.uv;
    float4 src = g_color;
    src.a = 1.0;
    const float4 dest = g_texture0.Sample(g_sampler0, uv);
    const float4 ret = softlight(dest, src);
    float2 diff = uv - float2(0.5, 0.5);
    diff.x *= 800.0 / 600.0; // aspect
    float factor = length(diff) * 0.8 / 0.5;
    const float4 result = lerp(ret, src, factor * 0.35);
    return lerp(dest, result, g_color.a * pow(factor, 3.2));
}
