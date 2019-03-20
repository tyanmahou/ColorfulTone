#include "KeyConfig.hpp"
#include "Useful.hpp"
#include<Siv3D.hpp>

namespace
{
	//キー変更キーボード
	//重要なキーを選べないようにするためにはstrictをtrueに(DownがUpになるとカーソルが動かせなくなる)
	bool ChangeKeyForKeyboard(Key& key,bool strict=false)
	{
		for (auto newkey : Input::GetActiveKeys())
		{
			key = newkey;
			if (key == Input::KeyDelete)
			{
				key = Key(0x00, Key::VirtualKeyCode{});
			}
			if (strict)
			{
				if (key == Input::KeyUp || key == Input::KeyDown || key == Input::KeyRight || key == Input::KeyLeft || key == Input::KeyEnter)
				{
					key = Key(0x00, Key::VirtualKeyCode{});
				}
			}
			return true;
		}
		return false;
	}
	//キー変更キーコンフィグ
	bool ChangeKeyForGamePad(Key& key)
	{
		for (auto&& gamepadInfo : Input::EnumerateGamepads())
		{
			const auto gamepad = Gamepad(gamepadInfo.index);
			if (gamepad.hasPOV)
			{
				const std::array<Key, 4> povKeys = {
					gamepad.povBackward,
					gamepad.povForward,
					gamepad.povLeft,
					gamepad.povRight
				};
				for (const auto& k : povKeys)
				{
					if (k.clicked)
					{
						key = k;
						return true;
					}
				}
			}
			for (unsigned int i = 0; i < gamepadInfo.num_buttons; ++i)
			{
				if (gamepad.button(i).pressed)
				{
					key = gamepad.button(i);
					return true;
				}
			}
		}
		return false;
	}

}

namespace
{
	String ForKeyboard(Key& key)
	{
		switch (key.code)
		{
		case 0x00:return L"----";
		case 0x01:return L"MouseL";
		case 0x02:return L"MouseR";
		case 0x04:return L"MouseM";
		case 0x05:return L"MouseX1";
		case 0x06:return L"MouseX2";
		case 0x03:return L"Cancel";
		case 0x08:return L"Backspace";
		case 0x09:return L"Tab";
		case 0x0C:return L"Clear";
		case 0x0D:return L"Enter";
		case 0x10:return L"Shift";
		case 0x11:return L"Control";
		case 0x12:return L"Alt";
		case 0x13:return L"Pause";
		case 0x1B:return L"Escape";
		case 0x20:return L"Space";
		case 0x21:return L"PageUp";
		case 0x22:return L"PageDown";
		case 0x23:return L"End";
		case 0x24:return L"Home";
		case 0x25:return L"Left";
		case 0x26:return L"Up";
		case 0x27:return L"Right";
		case 0x28:return L"Down";
		case 0x2C:return L"PrintScreen";
		case 0x2D:return L"Insert";
		case 0x2E:return L"Delete";
		case 0xA0:return L"LShift";
		case 0xA1:return L"RShift";
		case 0xA2:return L"LControl";
		case 0xA3:return L"RControl";
		case 0xA4:return L"LAlt";
		case 0xA5:return L"RAlt";
		case 0x30:return L"0";
		case 0x31:return L"1";
		case 0x32:return L"2";
		case 0x33:return L"3";
		case 0x34:return L"4";
		case 0x35:return L"5";
		case 0x36:return L"6";
		case 0x37:return L"7";
		case 0x38:return L"8";
		case 0x39:return L"9";
		case 0x41:return L"A";
		case 0x42:return L"B";
		case 0x43:return L"C";
		case 0x44:return L"D";
		case 0x45:return L"E";
		case 0x46:return L"F";
		case 0x47:return L"G";
		case 0x48:return L"H";
		case 0x49:return L"I";
		case 0x4A:return L"J";
		case 0x4B:return L"K";
		case 0x4C:return L"L";
		case 0x4D:return L"M";
		case 0x4E:return L"N";
		case 0x4F:return L"O";
		case 0x50:return L"P";
		case 0x51:return L"Q";
		case 0x52:return L"R";
		case 0x53:return L"S";
		case 0x54:return L"T";
		case 0x55:return L"U";
		case 0x56:return L"V";
		case 0x57:return L"W";
		case 0x58:return L"X";
		case 0x59:return L"Y";
		case 0x5A:return L"Z";
		case 0x60:return L"Num0";
		case 0x61:return L"Num1";
		case 0x62:return L"Num2";
		case 0x63:return L"Num3";
		case 0x64:return L"Num4";
		case 0x65:return L"Num5";
		case 0x66:return L"Num6";
		case 0x67:return L"Num7";
		case 0x68:return L"Num8";
		case 0x69:return L"Num9";
		case 0x6A:return L"NumMultiply";
		case 0x6B:return L"NumAdd";
		case 0x6C:return L"NumSeparator";
		case 0x6D:return L"NumSubtract";
		case 0x6E:return L"NumDecimal";
		case 0x6F:return L"NumDivide";
		case 0x90:return L"NumLock";
		case 0x70:return L"F1";
		case 0x71:return L"F2";
		case 0x72:return L"F3";
		case 0x73:return L"F4";
		case 0x74:return L"F5";
		case 0x75:return L"F6";
		case 0x76:return L"F7";
		case 0x77:return L"F8";
		case 0x78:return L"F9";
		case 0x79:return L"F10";
		case 0x7A:return L"F11";
		case 0x7B:return L"F12";
		case 0xBA:return L"Colon";
		case 0xBB:return L"Semicolon";
		case 0xBC:return L"Comma";
		case 0xBD:return L"Minus";
		case 0xBE:return L"Period";
		case 0xBF:return L"Slash";
		case 0xC0:return L"At";
		case 0xDB:return L"LBracket";
		case 0xDC:return L"Yen";
		case 0xDD:return L"RBracket";
		case 0xDE:return L"Caret";
		case 0xE2:return L"Backslash";
		case 0xB0:return L"NextTrack";
		case 0xB1:return L"PreviousTrack";
		case 0xB2:return L"StopMedia";
		case 0xB3:return L"PlayPauseMedia";
		}
		return  Format(L"Keyboard_Button_", key.code);
	}
	String ForGamepad(Key& key)
	{
		return Format(L"Gamepad_Button_", key.code);
	}
	String ForXInput(Key& key)
	{
		return Format(L"ForXInput_Button_", key.code);
	}

	String GetKeyName(Key& key)
	{
		if (key.device == KeyDeviceType::Keyboard)
			return ForKeyboard(key);
		if (key.device == KeyDeviceType::Gamepad)
			return ForGamepad(key);
		if (key.device == KeyDeviceType::XInput)
			return ForXInput(key);

		return L"None";
	}
}

//-----------------------------------------------------------------------

class PlayKeyConfig;
class DetailKeyConfig;
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//内部実装
//-----------------------------------------------------------------------

class KeyConfig::Impl
{
private:
	KeyConfig* m_pKeyConfig;
protected:
	enum class Mode :unsigned int
	{
		ChangeKey,
		Select,
	}m_mode;

	int m_timer;
	Font m_font;

	virtual bool selectChange() = 0;
	virtual void changeKey() = 0;

	template<class Type>
	void changeMode()
	{
		m_pKeyConfig->changeMode<Type>();
	}
public:
	Impl(int fontSize = 20) :
		m_timer(0),
		m_font(fontSize),
		m_mode(Mode::Select)
	{}


	bool update()
	{
		++m_timer;
		if (m_mode == Mode::Select)//選択モードなら選択処理
		{
			if (!this->selectChange())
				return false;
		}
		else
		{
			this->changeKey();	//チェンジモードならキーの変更処理
		}

		return true;
	}
	virtual void draw()const = 0;

	void setKeyConfig(KeyConfig* pKeyConfig)
	{
		this->m_pKeyConfig = pKeyConfig;
	}
};

//-----------------------------------------------------------------------
//実際のゲームプレイに使うキーのコンフィグ
//-----------------------------------------------------------------------

class PlayKeyConfig :public KeyConfig::Impl
{
	enum class Select :unsigned int
	{
		Red1, Red2, Red3,
		Blue1, Blue2, Blue3,
		Yellow1, Yellow2, Yellow3,
		Detail,
		Default,
		Escape,
	};
private:
	Select m_select;
	//選択の移動
	Select moveUp(Select select)
	{
		switch (select)
		{
		case Select::Red1:return Select::Default;
		case Select::Red2:return Select::Red1;
		case Select::Red3:return Select::Red2;

		case Select::Blue1:return Select::Red3;
		case Select::Blue2:return Select::Blue1;
		case Select::Blue3:return Select::Blue2;

		case Select::Yellow1:return Select::Escape;
		case Select::Yellow2:return Select::Yellow1;
		case Select::Yellow3:return Select::Yellow2;

		case Select::Detail:return Select::Blue3;
		case Select::Default:return Select::Detail;
		case Select::Escape:return Select::Blue3;
		default:
			break;
		}
		return Select::Red1;
	}
	Select moveDown(Select select)
	{
		switch (select)
		{
		case Select::Red1:return Select::Red2;
		case Select::Red2:return Select::Red3;
		case Select::Red3:return Select::Blue1;

		case Select::Blue1:return Select::Blue2;
		case Select::Blue2:return Select::Blue3;
		case Select::Blue3:return Select::Escape;

		case Select::Yellow1:return Select::Yellow2;
		case Select::Yellow2:return Select::Yellow3;
		case Select::Yellow3:return Select::Blue1;

		case Select::Detail:return Select::Default;
		case Select::Default:return Select::Red1;
		case Select::Escape:return Select::Yellow1;
		default:
			break;
		}
		return Select::Red1;

	}
	Select moveRight(Select select)
	{
		switch (select)
		{
		case Select::Red1:return Select::Yellow1;
		case Select::Red2:return Select::Yellow2;
		case Select::Red3:return Select::Yellow3;

		case Select::Blue1:return Select::Yellow2;
		case Select::Blue2:return Select::Escape;
		case Select::Blue3:return Select::Escape;

		case Select::Yellow1:return Select::Red1;
		case Select::Yellow2:return Select::Red2;
		case Select::Yellow3:return Select::Red3;

		case Select::Detail:return Select::Blue2;
		case Select::Default:return Select::Blue3;
		case Select::Escape:return Select::Detail;
		default:
			break;
		}
		return Select::Red1;
	}
	Select moveLeft(Select select)
	{
		switch (select)
		{
		case Select::Red1:return Select::Yellow1;
		case Select::Red2:return Select::Yellow2;
		case Select::Red3:return Select::Yellow3;

		case Select::Blue1:return Select::Red2;
		case Select::Blue2:return Select::Detail;
		case Select::Blue3:return Select::Default;

		case Select::Yellow1:return Select::Red1;
		case Select::Yellow2:return Select::Red2;
		case Select::Yellow3:return Select::Red3;

		case Select::Detail:return Select::Escape;
		case Select::Default:return Select::Escape;
		case Select::Escape:return Select::Blue2;
		default:
			break;
		}
		return Select::Red1;
	}

	bool selectChange()override
	{
		if (PlayKey::Right().clicked)
		{
			SoundManager::SE::Play(L"select");
			m_select = moveRight(m_select);
		}
		else if (PlayKey::Left().clicked)
		{
			SoundManager::SE::Play(L"select");
			m_select = moveLeft(m_select);
		}
		else if (PlayKey::Up().clicked)
		{
			SoundManager::SE::Play(L"select");
			m_select = moveUp(m_select);
		}
		else if (PlayKey::Down().clicked)
		{
			SoundManager::SE::Play(L"select");
			m_select = moveDown(m_select);
		}
		if (PlayKey::Start().clicked)
		{
			SoundManager::SE::Play(L"desisionSmall");

			if (m_select <= Select::Yellow3)
			{
				m_mode = Mode::ChangeKey;
			}
			else if (m_select == Select::Detail)
			{
				this->changeMode<DetailKeyConfig>();
			}
			else if (m_select == Select::Default)
			{
				PlayKey::Reset();
			}
			else if (m_select == Select::Escape)
			{
				return false;
			}
		}
		if (PlayKey::SmallBack().clicked)
			return false;

		return true;
	}

	void changeKey()override
	{
		auto& c = Game::Config();
		bool isChange = false;
		if (Input::AnyKeyClicked())
		{
			switch (m_select)
			{
			case Select::Red1:isChange = ChangeKeyForKeyboard(c.m_red1); break;
			case Select::Red2:isChange = ChangeKeyForKeyboard(c.m_red2); break;
			case Select::Red3:isChange = ChangeKeyForKeyboard(c.m_red3); break;

			case Select::Blue1:isChange = ChangeKeyForKeyboard(c.m_blue1); break;
			case Select::Blue2:isChange = ChangeKeyForKeyboard(c.m_blue2); break;
			case Select::Blue3:isChange = ChangeKeyForKeyboard(c.m_blue3); break;

			case Select::Yellow1:isChange = ChangeKeyForKeyboard(c.m_yellow1); break;
			case Select::Yellow2:isChange = ChangeKeyForKeyboard(c.m_yellow2); break;
			case Select::Yellow3:isChange = ChangeKeyForKeyboard(c.m_yellow3); break;

			default:
				break;
			}
		}

		if (!isChange)
		{
			switch (m_select)
			{
			case Select::Red1:isChange = ChangeKeyForGamePad(c.m_red1); break;
			case Select::Red2:isChange = ChangeKeyForGamePad(c.m_red2); break;
			case Select::Red3:isChange = ChangeKeyForGamePad(c.m_red3); break;

			case Select::Blue1:isChange = ChangeKeyForGamePad(c.m_blue1); break;
			case Select::Blue2:isChange = ChangeKeyForGamePad(c.m_blue2); break;
			case Select::Blue3:isChange = ChangeKeyForGamePad(c.m_blue3); break;

			case Select::Yellow1:isChange = ChangeKeyForGamePad(c.m_yellow1); break;
			case Select::Yellow2:isChange = ChangeKeyForGamePad(c.m_yellow2); break;
			case Select::Yellow3:isChange = ChangeKeyForGamePad(c.m_yellow3); break;
			default:
				break;
			}
		}
		if (isChange)
		{
			PlayKey::Update();
			SoundManager::SE::Play(L"desisionSmall");
			m_mode = Mode::Select;
		}
	}


	void draw()const
	{
		TextureAsset(L"keyconBG2").draw();

		const auto getColor = [=](Select s)
		{
			if (m_select == s)
			{
				if (m_mode == Mode::ChangeKey)
					return Color(255, 0, 0, 128 * ((m_timer / 10) % 2));
				else
					return Palette::Red;
			}

			return Palette::White;
		};

		auto& c = Game::Config();
		{
			constexpr int y = 155;
			m_font(GetKeyName(c.m_red1)).drawCenter(170, y, getColor(Select::Red1));
			m_font(GetKeyName(c.m_red2)).drawCenter(170, y + 60, getColor(Select::Red2));
			m_font(GetKeyName(c.m_red3)).drawCenter(170, y + 120, getColor(Select::Red3));
		}
		{
			constexpr int y = 180;
			m_font(GetKeyName(c.m_blue1)).drawCenter(400, 600 - y, getColor(Select::Blue1));
			m_font(GetKeyName(c.m_blue2)).drawCenter(400, 600 - y + 60, getColor(Select::Blue2));
			m_font(GetKeyName(c.m_blue3)).drawCenter(400, 600 - y + 120, getColor(Select::Blue3));
		}
		{
			constexpr int y = 155;
			m_font(GetKeyName(c.m_yellow1)).drawCenter(800 - 170, y, getColor(Select::Yellow1));
			m_font(GetKeyName(c.m_yellow2)).drawCenter(800 - 170, y + 60, getColor(Select::Yellow2));
			m_font(GetKeyName(c.m_yellow3)).drawCenter(800 - 170, y + 120, getColor(Select::Yellow3));
		}
		m_font(L"詳細設定").drawCenter(130, 490, getColor(Select::Detail));
		m_font(L"初期設定").drawCenter(130, 540, getColor(Select::Default));

		m_font(L"戻る").drawCenter(800 - 130, 525, getColor(Select::Escape));



		if (PlayKey::Red().pressed)
			TextureAsset(L"center_redlight").drawAt(400, 300);

		if (PlayKey::Blue().pressed)
			TextureAsset(L"center_bluelight").drawAt(400, 300);

		if (PlayKey::Yellow().pressed)
			TextureAsset(L"center_yellowlight").drawAt(400, 300);


		TextureAsset(L"center_base").drawAt(400, 300);

		static const Color judgeLineColor(255, 165, 0, 255);

		//判定円
		Circle(400, 300, 40).drawFrame(2, 2, judgeLineColor);
	}
public:
	PlayKeyConfig() :
		m_select(Select::Red1)
	{}
};

//詳細キーのコンフィグ
class DetailKeyConfig :public KeyConfig::Impl
{
	enum Select
	{
		Up, Down, Left, Right,
		Start, SmallBack, BigBack,
		SIZE
	};
private:

	Select m_select;
	bool selectChange()override
	{
		if (PlayKey::Down().clicked)
		{
			SoundManager::SE::Play(L"select");

			if (m_select < Select::SIZE - 1)
			{

				m_select = static_cast<Select>(m_select + 1);
			}
		}
		if (PlayKey::Up().clicked)
		{
			SoundManager::SE::Play(L"select");

			if (m_select > 0)
			{
				m_select = static_cast<Select>(m_select - 1);
			}
		}
		if (PlayKey::Start().clicked)
		{
			SoundManager::SE::Play(L"desisionSmall");

			m_mode = Mode::ChangeKey;
		}
		if (PlayKey::SmallBack().clicked)
		{
			SoundManager::SE::Play(L"cancel");
			this->changeMode<PlayKeyConfig>();
		}

		return true;
	}
	void changeKey()override
	{
		auto& c = Game::Config();
		bool isChange = false;
		if (Input::AnyKeyClicked())
		{
			switch (m_select)
			{
			case Select::Up:		isChange = ChangeKeyForKeyboard(c.m_up,true); break;
			case Select::Down:		isChange = ChangeKeyForKeyboard(c.m_down, true); break;
			case Select::Left:		isChange = ChangeKeyForKeyboard(c.m_left, true); break;
			case Select::Right:		isChange = ChangeKeyForKeyboard(c.m_right, true); break;

			case Select::Start:		isChange = ChangeKeyForKeyboard(c.m_start, true); break;
			case Select::SmallBack:	isChange = ChangeKeyForKeyboard(c.m_smallBack, true); break;
			case Select::BigBack:	isChange = ChangeKeyForKeyboard(c.m_bigBack, true); break;

			default:
				break;
			}
		}

		if (!isChange)
		{
			switch (m_select)
			{
			case Select::Up:		isChange = ChangeKeyForGamePad(c.m_up); break;
			case Select::Down:		isChange = ChangeKeyForGamePad(c.m_down); break;
			case Select::Left:		isChange = ChangeKeyForGamePad(c.m_left); break;
			case Select::Right:		isChange = ChangeKeyForGamePad(c.m_right); break;

			case Select::Start:		isChange = ChangeKeyForGamePad(c.m_start); break;
			case Select::SmallBack:	isChange = ChangeKeyForGamePad(c.m_smallBack); break;
			case Select::BigBack:	isChange = ChangeKeyForGamePad(c.m_bigBack); break;

			default:
				break;
			}
		}
		if (isChange)
		{
			PlayKey::Update();
			SoundManager::SE::Play(L"desisionSmall");
			m_mode = Mode::Select;
		}
	}
	void draw()const override
	{
		Rect(50, 100, 700, 450).draw(ColorF(0, 0.7));

		const auto getColor = [=](Select s)
		{
			if (m_select == s)
			{
				if (m_mode == Mode::ChangeKey)
					return Color(255, 0, 0, 128 * ((m_timer / 10) % 2));
				else
					return Palette::Red;
			}

			return Palette::White;
		};

		auto& c = Game::Config();
		{
			std::tuple<String, String, String, Color> list[7] =
			{
				{ L"↑", L"Up", GetKeyName(c.m_up), getColor(Select::Up) },
				{ L"↓", L"Down", GetKeyName(c.m_down), getColor(Select::Down) },
				{ L"←", L"Left", GetKeyName(c.m_left), getColor(Select::Left) },
				{ L"→", L"Right", GetKeyName(c.m_right), getColor(Select::Right) },
				{ L"決定", L"Enter", GetKeyName(c.m_start), getColor(Select::Start) },
				{ L"戻る(小)", L"Backspace", GetKeyName(c.m_smallBack), getColor(Select::SmallBack) },
				{ L"戻る(大)", L"Escape", GetKeyName(c.m_bigBack), getColor(Select::BigBack) },

			};
			int i = 0;
			for (auto&& elm : list)
			{
				m_font(std::get<0>(elm)).drawCenter(100, 125 + i * 50);
				m_font(std::get<1>(elm)).drawCenter(200, 125 + i * 50);
				m_font(std::get<2>(elm)).drawCenter(300, 125 + i * 50, std::get<3>(elm));

				++i;
			}
		}
	}
public:
	DetailKeyConfig() :
		KeyConfig::Impl(15),
		m_select(Select::Up)
	{}
};

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------

KeyConfig::KeyConfig()
{
	this->changeMode<PlayKeyConfig>();
}

bool KeyConfig::update()
{
	return m_pImpl->update();
}

void KeyConfig::draw() const
{
	m_pImpl->draw();
}

template<class Type>
void KeyConfig::changeMode()
{
	m_pImpl = std::make_shared<Type>();
	m_pImpl->setKeyConfig(this);
}

template void KeyConfig::changeMode<PlayKeyConfig>();
template void KeyConfig::changeMode<DetailKeyConfig>();
