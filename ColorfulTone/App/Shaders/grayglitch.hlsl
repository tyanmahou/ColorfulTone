Texture2D g_texture0 : register(t0);
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

float4 grayscale(float4 color)
{
    float3 rgb = dot(color.rgb, float3(0.299, 0.587, 0.114));
    return float4(rgb, color.a);
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

    float2 shiftOffs = float2(4, -2);
    float2 noise2 = rand2_1_1(uv + g_timer);
    float pt = saw(0.1 + 2.0 * rand(floor(uv.y * 20)), g_timer + 5.0 * rand(floor(uv.x * 20)));
    shiftOffs += 50 * noise2 * tri(0.1, lerp(0, pt, pt <= 0.1));
    float2 shiftUv = shiftOffs / size;
    
    float2 ra = grayscale(g_texture0.Sample(g_sampler0, uv + shiftUv)).ra;
    float2 ga = grayscale(g_texture0.Sample(g_sampler0, uv)).ga;
    float2 ba = grayscale(g_texture0.Sample(g_sampler0, uv - shiftUv)).ba;
    float a = (ra.y + ga.y + ba.y) / 3;
    return float4(ra.x, ga.x, ba.x, a);
}

float4 PS(PSInput input) : SV_TARGET
{
    float4 texColor = rgbShift(input.uv);
    return (texColor * input.color) + g_colorAdd;
}
