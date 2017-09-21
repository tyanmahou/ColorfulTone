#include"ColorfulTone.h"
#include"Game.h"

namespace
{

	bool CheckVersion()
	{
		const FilePath url = L"http://www11.atpages.jp/tyanmahou/game/ColorfulTone/Version.txt";

		if (!Internet::IsConnected()) // �C���^�[�l�b�g�ڑ����`�F�b�N
		{
			return false;
		}

		ByteArray reader;
		HTTPClient client;
		if (client.download(url, reader))
		{
			String str = TextReader(std::move(reader)).readAll();
			if (str != Game::Version)
				MessageBox::Show(L"�ŐV�̃o�[�W������" + str + L"�ł��B");

			return true;
		}
		return false;
	}
}

bool ColorfulTone::update()
{
	return Game::Instance()->updateAndDraw();
}

ColorfulTone::ColorfulTone():
	AppBase(L"ColorfulTone")
{

	Profiler::EnableWarning(false);

	//�o�[�W�����`�F�b�N
	::CheckVersion();

}

ColorfulTone::~ColorfulTone()
{}
