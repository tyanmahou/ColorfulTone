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
float4 PS(PSInput input) : SV_TARGET
{
    const float2 uv = input.uv;

    float4 color[10];
    for (int i = 0; i < 10; ++i)
    {
        const float size = fmod((g_timer / 100.0 + 192 * i), 1920);

        const float newSize = size * size / 1920.0;
        const float map = newSize > 20 ? 1920 / newSize : 1;
        const float alpha = 0.2 - 0.2 * abs(size - 960.0) / 960.0;
        const float2 offset = float2(0.5 + 1.0 / (map * 2.0), 0.5 + 1.0 / (map * 2.0));
        color[i] = g_texture0.Sample(g_sampler0, (uv - offset) * map) * alpha;
    }

    const float4 outColor = (color[0] + color[1] + color[2] + color[3] + color[4]
		+ color[5] + color[6] + color[7] + color[8] + color[9]);
    return (outColor * input.color) + g_colorAdd;
}