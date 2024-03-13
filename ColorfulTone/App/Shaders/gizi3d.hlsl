cbuffer VSConstants2D : register(b0)
{
    row_major float2x4 g_transform;
    float4 g_colorMul;
}
struct VSInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};
struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

cbuffer ShaderParam : register(b1)
{
    float2 g_sceneSize;
    float4x4 g_view;
}
float4 Transform2D(float2 pos, float2x4 t)
{
    return float4((t._13_14 + (pos.x * t._11_12) + (pos.y * t._21_22)), t._23_24);
}
PSInput VS(VSInput input)
{
    PSInput result;
    float4 pos = Transform2D(input.position, g_transform);
    pos.x = g_sceneSize.x * ((pos.x + 1.0) / 2.0);
    pos.y = g_sceneSize.y * (1 - ((pos.y + 1.0) / 2.0));
    result.position = mul(float4(pos.x, 0, -pos.y, 1), g_view);
    result.color = input.color * g_colorMul;
    result.uv = input.uv;
    return result;
}