#include "PlayBG.h"
#include "MusicData.h"
#include "Util.h"

void DefaultBG::init(const NotesData & notes)
{
	m_texture = notes.getMusic()->getTexture().resize(1920, 1920);
}

void DefaultBG::apply(double count) const
{
	static PixelShader ps(L"Shaders/mainBg.ps");
	static ConstantBuffer<Float4> cb;
	cb->set(static_cast<float>(count), 0, 0, 0);
	Graphics2D::SetConstant(ShaderStage::Pixel, 1, cb);
	Graphics2D::BeginPS(ps);
	m_texture.drawAt(400, 300);
	Graphics2D::EndPS();
}

BlurBG::BlurBG():
	m_texture(Window::BaseSize(), Palette::White)
{}

void BlurBG::init(const NotesData & notes)
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
		notes.getMusic()->getTexture().resize(800, 800).drawAt(Window::BaseCenter(), ColorF(1, 0.9));
	}
	Graphics2D::EndPS();

	Graphics2D::SetRenderTarget(Graphics::GetSwapChainTexture());
}

void BlurBG::apply(double count) const
{
	m_texture.draw();
}

SimpleBG::SimpleBG(const Color & color):
	m_color(color)
{
	m_color.setAlpha(240);
}

void SimpleBG::init(const NotesData & notes)
{
	m_texture = notes.getMusic()->getTexture().resize(600, 600);
}

void SimpleBG::apply(double count) const
{
	m_texture.drawAt(400, 300);

	Window::BaseClientRect().draw(m_color);
} 
