Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
SamplerState g_sampler0 : register(s0);

cbuffer PSConstants2D : register(b0)
{
    float4 g_colorAdd;
    float4 g_sdfParam;
    float4 g_sdfOutlineColor;
    float4 g_sdfShadowColor;
    float4 g_internal;
}

cbuffer ShaderParam : register(b1)
{
    float g_timer;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

float4 PS(PSInput input) : SV_TARGET
{
    const float2 uv = input.uv;
    float4 outPut = g_texture0.Sample(g_sampler0, uv);
    float4 mask = g_texture1.Sample(g_sampler0, uv);
    float timer = min(1.0, g_timer);
    if (mask.r <= timer)
    {
        outPut.a = 1.0 - mask.b;
    }
    else
    {
        outPut.a = 0;
    }
    return (outPut * input.color) + g_colorAdd;
}