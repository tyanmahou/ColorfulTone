#pragma once
#include<Siv3d.hpp>
#include"MusicData.h"
#include"Util.h"
enum class BGType
{
	Default=0,
	Blur,
	Black,
	White
};

class IPlayBG
{
public:
	IPlayBG() = default;
	virtual ~IPlayBG() = default;
	virtual void init(MusicData& musicData)= 0;
	virtual void apply(double count=0)const = 0;
};

class DefaultBG:public IPlayBG
{
public:
	DefaultBG() = default;
private:
	TextureRegion m_texture;

	void init(MusicData& musicData)override
	{
		m_texture = musicData.getTexture().resize(1920, 1920);
	}
	void apply(double count)const override
	{
		static PixelShader ps(L"Shaders/mainBg.ps");
		static ConstantBuffer<Float4> cb;
		cb->set(static_cast<float>(count), 0, 0, 0);
		Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
		Graphics2D::BeginPS(ps);
		m_texture.drawAt(400, 300);
		Graphics2D::EndPS();
	}
};

class BlurBG :public IPlayBG
{
public:
	BlurBG():
		m_texture(Window::BaseSize(), Palette::White)
	{}

private:

	RenderTexture m_texture;

	void init(MusicData& musicData)override
	{
		static PixelShader ps(L"Shaders/radialBlur.ps");
		static ConstantBuffer<Float4> cb;
		cb->set(5.0, 800.0, 800.0, 0);
		Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);

		Graphics2D::SetRenderTarget(m_texture);
		m_texture.clear(Palette::White);
		Graphics2D::BeginPS(ps);
		{
			util::Transformer2D t2d(Graphics2D::GetTransform().inverse());
			musicData.getTexture().resize(800, 800).drawAt(Window::BaseCenter(), ColorF(1, 0.9));
		}
		Graphics2D::EndPS();

		Graphics2D::SetRenderTarget(Graphics::GetSwapChainTexture());
	}
	void apply(double count)const override
	{
		m_texture.draw();
	}
};
class SimpleBG :public IPlayBG
{
public:
	SimpleBG(const Color& color = Palette::White) :
		m_color(color)
	{
		m_color.setAlpha(240);
	}
private:
	Color m_color;
	TextureRegion m_texture;

	void init(MusicData& musicData)override
	{
		m_texture = musicData.getTexture().resize(600, 600);
	}
	void apply(double count)const override
	{
		m_texture.drawAt(400, 300);

		Window::BaseClientRect().draw(m_color);

	}
};

inline std::shared_ptr<IPlayBG> CreateBG(BGType type=BGType::Default)
{
	switch (type)
	{
	case BGType::Default:
		return std::make_shared<DefaultBG>();
	case BGType::Blur:
		return std::make_shared<BlurBG>();
	case BGType::Black:
		return std::make_shared<SimpleBG>(Palette::Black);
	case BGType::White:
		return std::make_shared<SimpleBG>();
	default:
		break;
	}

	return std::make_shared<DefaultBG>();
}