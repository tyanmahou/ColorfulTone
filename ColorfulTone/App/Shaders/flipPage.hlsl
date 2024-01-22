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
    int g_in;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

float getTimer()
{
    return (1.0 - g_timer);
}
// ライン計算
float2 toFoldLine()
{
    float timer = getTimer();
    float t = 1 - timer;
    float tY = sqrt(1 - pow(t, 2));
    float a = -4 * tY / t;
    float b = 2.0 + 4 * sqrt(timer);
    return float2(a, b);
}
// y = ax + b に対する 対称点
float2 symmetric(float2 xy, float a, float b)
{
    float resultY = (2 * a * xy.x + (a * a - 1.0) * xy.y + 2 * b) / (1 + a * a);
    float resultX = xy.x - a * (resultY - xy.y);
    return float2(resultX, resultY);
}

// 補正がかかるか
bool hasCorrection(float2 xy, float2 uv, float a, float b)
{
    float x1y = a + b;
    float h = 1.0 - x1y;
    float cx = -0.03 * sin((uv.y - x1y) * 3.1415 / h) + 1.0;

    float xy1 = (1 - b) / a;
    float w = 1.0 - xy1;
    float cy = 0.03 * sin((uv.x - xy1) * 3.1415 / w) + 1.0;

    return xy.x >= cx || xy.y >= cy;
}

float4 PS(PSInput input) : SV_TARGET
{
    const float timer = getTimer();
    const float2 uv = input.uv;
    float4 srcColor = g_texture0.Sample(g_sampler0, uv);

    // 折り曲げラインを計算
    float2 l = toFoldLine();
    float a = l.x;
    float b = l.y;

    float y = a * uv.x + b;

    // ラインに対する対称点が画面内の場合
    float2 sym = symmetric(uv, a, b);

    // 補正
    const bool hasCor = hasCorrection(sym, uv, a, b);
    if (uv.y > y)
    {
        if (!g_in)
        {
            discard;
        }
        // ラインより内は
    }
    else if (!hasCor && sym.x <= 1.0 && sym.y <= 1.0)
    {
        const float4 backColor = float4(0.33, 0.3, 0.2, 1);
        srcColor = backColor;
    }
    else
    {
        if (g_in)
        {
            discard;
        }
    }
    return (srcColor * input.color) + g_colorAdd;
}
