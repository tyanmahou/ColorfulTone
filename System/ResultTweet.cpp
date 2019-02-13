#include"ResultTweet.h"
#include"Setting.hpp"
#include<Siv3D.hpp>
class ResultTweet::CTweet
{
	static TwitterClient& Twitter()
	{
		static 	s3d::TwitterClient twitter(Setting::ApiKey, Setting::ApiSecret);
		return twitter;
	}
private:
	s3d::GUI m_auth;
	s3d::GUI m_gui;
	Image m_img;
	Image m_thumbnail;

	void initAuthGUI()
	{
		//認証GUIの初期化
		m_auth.setTitle(L"PIN の入力");
		m_auth.addln(L"PIN", GUITextField::Create(7));
		m_auth.add(L"auth", GUIButton::Create(L"認証"));
		m_auth.add(L"cancel", GUIButton::Create(L"キャンセル"));
		m_auth.setPos({200,200});
		m_auth.show(false);
	}
	void updateThumbnail()
	{
		Imaging::Fit(m_img, m_thumbnail, 400, 300);
	}
	void initTweetGUI()
	{


		m_gui.setTitle(L"ツイート");
		m_gui.add(L"text", GUITextArea::Create(12, 12, 140));
		//ツイートGUIの初期化
		this->updateThumbnail();
		m_gui.addln(L"image", GUITexture::Create(Texture(m_thumbnail)));
		m_gui.add(L"tweet", GUIButton::Create(L"ツイート"));
		m_gui.add(L"cancel", GUIButton::Create(L"キャンセル"));

		m_gui.setPos({ 200,200 });
		m_gui.show(false);
	}
public:
	CTweet(const String& text) :
		m_auth(GUIStyle::Default),
		m_gui(GUIStyle::Default)
	{

		this->initAuthGUI();
		this->initTweetGUI();

		m_gui.textArea(L"text").setText(text);

	}

	bool update()
	{
		if (ScreenCapture::HasNewFrame())
		{
			m_img = ScreenCapture::GetFrame();
			this->updateThumbnail();
			m_gui.texture(L"image").texture = Texture(m_thumbnail);

			if (Twitter().authorized())
			{
				m_auth.show(false);
				m_gui.show();
			}
			else
			{
				Twitter().openTokenPage();
				m_auth.show(true);
			}
		}
		m_auth.button(L"auth").enabled = (m_auth.textField(L"PIN").text.length == 7);

		if (m_auth.button(L"auth").pushed && Twitter().verifyPIN(m_auth.textField(L"PIN").text))
		{
			m_auth.release();
			m_gui.show();
		}

		if (m_gui.button(L"tweet").pushed)
		{
			Twitter().tweetWithMedia(m_gui.textArea(L"text").text, m_img);
			return false;
		}
		if (m_gui.button(L"cancel").pushed)
		{
			return false;
		}
		return true;
	}

};
ResultTweet::ResultTweet()
{


}

bool ResultTweet::update()
{
	if (m_pImpl!=nullptr)
	{
		if (!m_pImpl->update())
		{
			m_pImpl = nullptr;
		}
		return true;
	}
	return false;
}

void ResultTweet::tweet(const String & text)
{
	if (m_pImpl == nullptr)
	{
		ScreenCapture::Request();
		m_pImpl = std::make_shared<CTweet>(text);
	}
}
