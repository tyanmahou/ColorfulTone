
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
	float g_count;
};

float getTimer()
{
	return g_count * 1.1;
}
// ライン計算
float2 toFoldLine()
{
	float timer = getTimer();
	float t = 1 - timer;
	float tY = sqrt(1 - pow(t, 2));
	float a = - 4 * tY / t;
	float b= 2.0 + 4 * sqrt(timer);
	return float2(a, b);
}
// y = ax + b に対する 対称点
float2 symmetric(float2 xy, float a, float b)
{
	float resultY = (2*a*xy.x + (a*a - 1.0)*xy.y + 2*b) / (1 + a*a);
	float resultX = xy.x - a * (resultY - xy.y);
	return float2(resultX, resultY);
}
float4 PS(VS_OUTPUT input) : SV_Target
{
	const float timer = getTimer();
	if(timer>=1.0){
		discard;
	}
	const float2 uv = input.tex;
	float4 srcColor = texture0.Sample(sampler0, uv);

	// 折り曲げラインを計算
	float2 l = toFoldLine();
	float a = l.x;
	float b = l.y;

	float y = a*uv.x+b;

	// ラインより外は破棄
	if(uv.y>y)
	{
		discard;
	}
	
	// ラインに対する対称点が画面内の場合
	float2 sym = symmetric(uv,a,b);

	// 補正
	float x1y = a+b;
	float h = 1.0-x1y;
	float ax = -0.03*sin((uv.y-x1y)*3.1415/h)+1.0;

	float xy1 = (1-b)/a;
	float w = 1.0-xy1;
	float ay = 0.03*sin((uv.x-xy1)*3.1415/w)+1.0;

	if(sym.x < ax && sym.y < ay)
	if(sym.x <= 1.0 && sym.y <= 1.0)
	{
		const float4 backColor = float4(0.33, 0.3, 0.2, 1);
		srcColor = backColor;	
	}
	return srcColor*input.color;
}
