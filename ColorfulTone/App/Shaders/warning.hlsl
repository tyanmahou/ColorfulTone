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
    float3 g_color;
    float g_blend;
    float g_noiseRate;
    float g_timer;
}

float softlight(float dest, float src)
{
    return src < 0.5 ? 2.0 * dest * src + dest * dest * (1.0 - 2.0 * src) : 2.0 * dest * (1 - src) + sqrt(dest) * (2 * src - 1.0);
}
float3 softlight(float3 dest, float3 src)
{
    float3 color;
    color.r = softlight(dest.r, src.r);
    color.g = softlight(dest.g, src.g);
    color.b = softlight(dest.b, src.b);
    return color;
}
float colorBurn(float dest, float src)
{
    return saturate(src <= 0 ? 0 : 1 - (1 - dest) / src);
}
float3 colorBurn(float3 dest, float3 src)
{
    float3 color;
    color.r = colorBurn(dest.r, src.r);
    color.g = colorBurn(dest.g, src.g);
    color.b = colorBurn(dest.b, src.b);
    return color;
}

float rand(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}
float blocknoise(float2 st)
{
    return rand(floor(st));

}
float blocknoise1_1(float2 st)
{
    return -1.0 + 2.0 * blocknoise(st);
}
float2 rand2(float2 st)
{
    st = float2(dot(st, float2(127.1, 311.7)), dot(st, float2(269.5, 183.3)));
    return frac(sin(st) * 43758.5453123);
}
float2 rand2_1_1(float2 st)
{
    return -1.0 + 2.0 * rand2(st);
}
float saw(float p, float t)
{
    return fmod(t, p) / p;
}
float tri(float p, float t)
{
    float x = fmod(t, p) / (p * 0.5);
    return lerp(2.0 - x, x, x <= 1.0);
}
float4 rgbShift(float2 uv)
{
    float2 size;
    float level;
    g_texture0.GetDimensions(0, size.x, size.y, level);

    // float t = (1 + sin(g_timer)) / 2.0;
    float2 shiftOffs = float2(2, -2);
    float2 noise2 = rand2_1_1(uv + g_timer);
    float pt = saw(0.1 + 2.0 * rand(floor(uv.y * 20)), g_timer + 5.0 * rand(floor(uv.x * 20)));
    shiftOffs += 50 * g_noiseRate * noise2 * tri(0.1, lerp(0, pt, pt <= 0.1));
    float2 shiftUv = shiftOffs / size;
    // + float2(-0.02, 0.0)
    float2 ra = g_texture0.Sample(g_sampler0, uv + shiftUv).ra;
    float2 ga = g_texture0.Sample(g_sampler0, uv).ga;
    float2 ba = g_texture0.Sample(g_sampler0, uv - shiftUv).ba;
    float a = (ra.y + ga.y + ba.y) / 3;
    return float4(ra.x, ga.x, ba.x, a);
}
float3 drop(float3 dest, float2 uv)
{
    const float3 ret = softlight(dest, g_color);
    
    float2 diff = uv - float2(0.5, 0.5);
    diff.x *= 800.0 / 600.0; // aspect
    float factor = length(diff) * 0.8 / 0.5;
    const float3 c = lerp(ret, g_color, factor * 0.35);
    
    float t = (1 + sin(g_timer * 4)) / 2.0;
    return lerp(dest, c, t * pow(factor, 3.2) * 0.8);
}

float4 PS(PSInput input) : SV_TARGET
{
    const float2 uv = input.uv;
    float4 src = g_texture0.Sample(g_sampler0, uv);
    float4 result = src;
    result = lerp(result, rgbShift(uv), g_noiseRate);
    
    // 焼きこみ
    {
        float t = 0.1 + 0.6 * (1 + sin(g_timer * 2)) / 2.0;
        float3 cb = colorBurn(result.rgb, g_color);
        result.rgb = lerp(result.rgb, cb, t);
    }
    result.rgb = drop(result.rgb, uv);
    result = lerp(src, result, g_blend);
    return (result * input.color) + g_colorAdd;
}
