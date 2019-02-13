#include"VideoAsset.h"
#include"StartAnime.h"
#include "Useful.hpp"

namespace StartAnime
{
	static VideoPlayer g_start;

	static PixelShader g_ps;

	bool Open()
	{
		if (!g_start)
		{
			g_start = Mahou::VideoAsset(L"start");
			g_ps = PixelShader(L"Shaders/startTrans.ps");
		}
		return g_start.operator bool();
	}
	void Play()
	{
		g_start.play();
	}
	void Stop()
	{
		g_start.stop();
	}
	void Draw()
	{
		if (g_start.isPlaying())
		{
			g_start.update();

			Graphics2D::BeginPS(g_ps);
			g_start.getFrameTexture().draw();

			Graphics2D::EndPS();
		}
	}

}