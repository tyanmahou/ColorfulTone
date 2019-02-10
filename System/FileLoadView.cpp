#include "FileLoadView.hpp"
#include"BGLight.hpp"
#include"SharedDraw.hpp"
class FileLoadView::Impl
{
private:
	Stopwatch m_stopwatch;
	String m_message;
	SharedDraw::DrawBGLight m_lights;
	bool m_isCompleted = false;
public:
	Impl() :
		m_message(L"NOW LOADING...")
	{
		m_stopwatch.start();
	}
	void update()
	{
		m_lights.update();
		// now loadingのインターバル時間
		constexpr int nowLoadingInterval = 4;
		if (m_stopwatch.s() > nowLoadingInterval) {
			m_stopwatch.restart();
		}
	}
	void draw()const
	{
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();

		const Vec2 center = Window::BaseCenter();
		const int timer = System::FrameCount() % 360;

		const double rad = static_cast<double>(timer) / 360 * TwoPi;
		const double startAngle = rad;
		const double mod = Fmod(rad, 1.0);
		const double angle = TwoPi * Pow(2.0 * (mod > 1.0 ? 1.0 : mod) - 1.0, 3);

		if (m_isCompleted) {
			const double size = static_cast<double>(Min(m_stopwatch.ms(), 350));
			Line(center - Vec2{ -size,0 }, center - Vec2{ size,0 }).draw(ColorF(0, 0.5));
			Circle(center, 200 + m_stopwatch.ms()).drawFrame(0, 1.2, ColorF(0, 0.5));
		}
		else {
			Circle(center, 200).drawArc(startAngle, angle, 0, 1.2, ColorF(0, 0.5));
		}
		const double index = m_stopwatch.ms() / (m_isCompleted ? 50 : 160.0);

		const auto animation = [&index](KineticTypography& k)
		{
			const double t = index - k.index;

			k.col.a = static_cast<uint32_t>(Saturate(t / 6) * 255);
			if (k.ch == ' ') {
				k.col.a = 0;
			}

			k.pos.y -= Easing::EaseIn(Easing::Quad, Saturate(1 - t / 4)) * 20.0;
		};
		//現在の文
		const auto offset = FontAsset(L"loading")(m_message).region().size / 2.0;
		FontAsset(L"judge")(m_message).drawKinetic(center - offset, animation, Palette::Black);
	}

	void onCompleted()
	{
		m_isCompleted = true;
		m_stopwatch.restart();
		m_message = L"COMPLETED";
	}
	int32 getStopwatchMs() const
	{
		return m_stopwatch.ms();
	}
};
FileLoadView::FileLoadView() :
	m_pImpl(std::make_shared<Impl>())
{}

FileLoadView::~FileLoadView()
{}

void FileLoadView::update()
{
	m_pImpl->update();
}
void FileLoadView::draw() const
{
	m_pImpl->draw();
}

void FileLoadView::onCompleted()
{
	m_pImpl->onCompleted();
}

int32 FileLoadView::getStopwatchMs() const
{
	return m_pImpl->getStopwatchMs();
}