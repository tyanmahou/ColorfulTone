#include"TitleScene.h"
#include"Fade.h"
#include"VideoAsset.h"
#include"SceneInfo.h"
#include"FontKinetic.h"
#include"PlayKey.h"
//--------------------------------------------------------------------------------
//�֐��F�R���X�g���N�^
//--------------------------------------------------------------------------------

TitleScene::TitleScene():
	m_timer(0),
	m_font(10, L"Straight",FontStyle::Outline),
	m_mode(Mode::GameStart)
{
	m_font.changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::White, 2));
	if(!SoundAsset(L"title").isPlaying())
	SoundAsset(L"title").play();
}

//--------------------------------------------------------------------------------
//�֐��F���Z�q�I�[�o�[���[�h
//--------------------------------------------------------------------------------

TitleScene::Mode operator ++(TitleScene::Mode& mode)
{
	mode = (mode == TitleScene::Mode::Exit) ? TitleScene::Mode::GameStart : TitleScene::Mode(static_cast<int>(mode) + 1);
	return mode;
}
TitleScene::Mode operator --(TitleScene::Mode& mode)
{
	mode = (mode == TitleScene::Mode::GameStart) ? TitleScene::Mode::Exit : TitleScene::Mode(static_cast<int>(mode)-1);
	return mode;
}
//--------------------------------------------------------------------------------
//�֐��Fupdate
//--------------------------------------------------------------------------------

namespace 
{

	void AccessHomePage()
	{
		const FilePath url = L"http://www11.atpages.jp/tyanmahou/game/ColorfulTone/index.php";

		if (Internet::IsConnected()) // �C���^�[�l�b�g�ڑ����`�F�b�N
		{
			Internet::LaunchWebBrowser(url);
		}
		else
		{
			MessageBox::Show(L"�C���^�[�l�b�g�ɐڑ��ł��܂���ł����B");
		}

	}
}
void TitleScene::update() 
{
	m_timer++;
	if (PlayKey::Up().clicked)
	{
		SoundAsset(L"select").playMulti(0.5);
		--m_mode;
	}
	else 	if (PlayKey::Down().clicked)
	{
		SoundAsset(L"select").playMulti(0.5);
		++m_mode;
	}
	if (PlayKey::Start().clicked)
	{
		SoundAsset(L"desisionLarge").playMulti(0.5);
		switch (m_mode)
		{
		case Mode::GameStart:
			changeScene(L"select", 3000); break;
		case Mode::Course:
			changeScene(L"courseSelect", 3000); break;
		case Mode::KeyConfig:
			changeScene(L"config", 3000); break;
		case Mode::Tutorial:
			changeScene(L"tutorial", 3000); break;
		case Mode::Reload:
			Game::Instance()->m_isMusicLoadEnd = false;
			changeScene(L"load", 3000); break;
		case Mode::Download :
			if (MessageBox::Show(L"�C���^�[�l�b�g�ɐڑ����z�[���y�[�W�ɃA�N�Z�X���܂��B",MessageBoxStyle::OkCancel) == MessageBoxCommand::Ok)
			::AccessHomePage(); break;
		case Mode::Exit:
			System::Exit(); break;

		default:
			break;
		}
	}

	if (PlayKey::BigBack().clicked)
	{
		SoundAsset(L"select").playMulti(0.5);
		m_mode = Mode::Exit;
	}

}
//--------------------------------------------------------------------------------
//�֐��Fdraw
//--------------------------------------------------------------------------------
void TitleScene::draw()const
{
	static auto v = Mahou::VideoAsset(L"titleBG");

	if (!v.isPlaying())
		v.play();
	v.update();
	v.getFrameTexture().draw();

	TextureAsset(L"logo").scale(0.8).drawAt(400,150);

	static Array<std::pair<String,String>> name = {
		{ L"iconPlay",L"FREE PLAY"},
		{ L"iconPlayCourse",L"COURSE PLAY" },
		{ L"iconConfig",L"CONFIG"},
		{ L"iconReset",L"RELOAD"},
		{ L"iconTutorial",L"TUTORIAL" },
		{ L"iconAccess",L"ACCESS" },
		{ L"iconShutdown",L"EXIT",}
	};

	constexpr double x = 400;
	//constexpr double y = 300;

	//for (unsigned int i = 0; i <= static_cast<unsigned int>(Mode::Exit); ++i) 
	//{
	//	const bool isSelect = m_mode == static_cast<Mode>(i);
	//	const int off = 40;
	//	if(isSelect)
	//		TextureAsset(L"modeBack").drawAt(x, y + off * i,ColorF(0.8,1,1,0.9+0.1*Sin(static_cast<double>(m_timer)/30.0)));
	//	
	//	const auto size=m_font(name[i].second).region().size;
	//	m_font(name[i].second).drawKinetic(x-size.x/2.0, y + off * i-size.y/2.0, FontKinetic::DeleteSpace,Palette::Black);

	//	TextureAsset(name[i].first).scale(0.15+ (isSelect?0.012*Sin(static_cast<double>(m_timer) / 20.0):0)).drawAt(x-150, y + off * i);
	//}

	constexpr double y = 350;
	constexpr int off = 40;

	TextureAsset(L"modeBack").drawAt(x, y + 40 * 2, ColorF(0.8, 1, 1, 0.9 + 0.1*Sin(static_cast<double>(m_timer) / 30.0)));

	for (unsigned int i = 0; i <5; ++i)
	{
		const unsigned index = (static_cast<unsigned int>(m_mode)+i + 5 )% 7;
		const auto size=m_font(name[index].second).region().size;
		m_font(name[index].second).drawKinetic(x-size.x/2.0, y + off*i -size.y/2.0, FontKinetic::DeleteSpace,ColorF(0,i==2?1:0.5-0.1*Abs<int>(2-i)));

		TextureAsset(name[index].first).scale(0.15+ (i==2?0.012*Sin(static_cast<double>(m_timer) / 20.0):0)).drawAt(x-150, y + off * i, ColorF(i==2, 1 - 0.3*Abs<int>(2 - i)));
	}


	SceneInfo::Draw(L"Press Enter");
	FontAsset(L"info")(Game::Version).draw(10, 600 - 40+ 10);
}

//--------------------------------------------------------------------------------
//�֐��FdrawFadeIn
//--------------------------------------------------------------------------------
void TitleScene::drawFadeIn(double t) const
{

	draw();
	FadeIn(t, Fade::SmoothCircle);

}

//--------------------------------------------------------------------------------
//�֐��FdrawFadeOut
//--------------------------------------------------------------------------------
void TitleScene::drawFadeOut(double t) const
{

	draw();
	FadeOut(t, Fade::SmoothCircle);

}