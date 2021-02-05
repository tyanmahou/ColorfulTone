#include"KeyConfig.hpp"

namespace s3dkc
{
	namespace
	{
		template<class Range, class T>
		bool InArray(const T& value, const Range& range)
		{
			auto endIt = std::end(range);
			return std::find(std::begin(range), endIt, value) != endIt;
		}
		//キーボードのキー名
		const String GetKeyboardName(const uint8 code)
		{
			static std::unordered_map<uint8, String> dictionary
			{
				{ 0x00,L"----" },
				{ 0x01,L"MouseL" },
				{ 0x02,L"MouseR" },
				{ 0x04,L"MouseM" },
				{ 0x05,L"MouseX1" },
				{ 0x06,L"MouseX2" },
				{ 0x03,L"Cancel" },
				{ 0x08,L"Backspace" },
				{ 0x09,L"Tab" },
				{ 0x0C,L"Clear" },
				{ 0x0D,L"Enter" },
				{ 0x10,L"Shift" },
				{ 0x11,L"Control" },
				{ 0x12,L"Alt" },
				{ 0x13,L"Pause" },
				{ 0x1B,L"Escape" },
				{ 0x20,L"Space" },
				{ 0x21,L"PageUp" },
				{ 0x22,L"PageDown" },
				{ 0x23,L"End" },
				{ 0x24,L"Home" },
				{ 0x25,L"Left" },
				{ 0x26,L"Up" },
				{ 0x27,L"Right" },
				{ 0x28,L"Down" },
				{ 0x2C,L"PrintScreen" },
				{ 0x2D,L"Insert" },
				{ 0x2E,L"Delete" },
				{ 0xA0,L"LShift" },
				{ 0xA1,L"RShift" },
				{ 0xA2,L"LControl" },
				{ 0xA3,L"RControl" },
				{ 0xA4,L"LAlt" },
				{ 0xA5,L"RAlt" },
				{ 0x30,L"0" },
				{ 0x31,L"1" },
				{ 0x32,L"2" },
				{ 0x33,L"3" },
				{ 0x34,L"4" },
				{ 0x35,L"5" },
				{ 0x36,L"6" },
				{ 0x37,L"7" },
				{ 0x38,L"8" },
				{ 0x39,L"9" },
				{ 0x41,L"A" },
				{ 0x42,L"B" },
				{ 0x43,L"C" },
				{ 0x44,L"D" },
				{ 0x45,L"E" },
				{ 0x46,L"F" },
				{ 0x47,L"G" },
				{ 0x48,L"H" },
				{ 0x49,L"I" },
				{ 0x4A,L"J" },
				{ 0x4B,L"K" },
				{ 0x4C,L"L" },
				{ 0x4D,L"M" },
				{ 0x4E,L"N" },
				{ 0x4F,L"O" },
				{ 0x50,L"P" },
				{ 0x51,L"Q" },
				{ 0x52,L"R" },
				{ 0x53,L"S" },
				{ 0x54,L"T" },
				{ 0x55,L"U" },
				{ 0x56,L"V" },
				{ 0x57,L"W" },
				{ 0x58,L"X" },
				{ 0x59,L"Y" },
				{ 0x5A,L"Z" },
				{ 0x60,L"Num0" },
				{ 0x61,L"Num1" },
				{ 0x62,L"Num2" },
				{ 0x63,L"Num3" },
				{ 0x64,L"Num4" },
				{ 0x65,L"Num5" },
				{ 0x66,L"Num6" },
				{ 0x67,L"Num7" },
				{ 0x68,L"Num8" },
				{ 0x69,L"Num9" },
				{ 0x6A,L"NumMultiply" },
				{ 0x6B,L"NumAdd" },
				{ 0x6C,L"NumSeparator" },
				{ 0x6D,L"NumSubtract" },
				{ 0x6E,L"NumDecimal" },
				{ 0x6F,L"NumDivide" },
				{ 0x90,L"NumLock" },
				{ 0x70,L"F1" },
				{ 0x71,L"F2" },
				{ 0x72,L"F3" },
				{ 0x73,L"F4" },
				{ 0x74,L"F5" },
				{ 0x75,L"F6" },
				{ 0x76,L"F7" },
				{ 0x77,L"F8" },
				{ 0x78,L"F9" },
				{ 0x79,L"F10" },
				{ 0x7A,L"F11" },
				{ 0x7B,L"F12" },
				{ 0xBA,L"Colon" },
				{ 0xBB,L"Semicolon" },
				{ 0xBC,L"Comma" },
				{ 0xBD,L"Minus" },
				{ 0xBE,L"Period" },
				{ 0xBF,L"Slash" },
				{ 0xC0,L"At" },
				{ 0xDB,L"LBracket" },
				{ 0xDC,L"Yen" },
				{ 0xDD,L"RBracket" },
				{ 0xDE,L"Caret" },
				{ 0xE2,L"Backslash" },
				{ 0xB0,L"NextTrack" },
				{ 0xB1,L"PreviousTrack" },
				{ 0xB2,L"StopMedia" },
				{ 0xB3,L"PlayPauseMedia" }
			};
			if (dictionary.find(code) != dictionary.end())
			{
				return dictionary.at(code);
			}

			return  Format(L"Keyboard_", code);
		}

		//ゲームパッドのキー名
		const String GetGamepadName(const uint8 userIndex, const uint8 code)
		{
			return Format(L"Gamepad_", userIndex, L"_", code);
		}

		//XInputのキー名
		const String GetXInputName(const uint8 userIndex, const uint8 code)
		{
			static std::unordered_map<uint8, String> dictionary
			{
				{ 0x1000 ,L"A"},
				{ 0x2000 ,L"B" },
				{ 0x4000 ,L"X" },
				{ 0x8000 ,L"Y" },
				{ 0x0001 ,L"Up" },
				{ 0x0002 ,L"Down" },
				{ 0x0004 ,L"Left" },
				{ 0x0008 ,L"Right" },
				{ 0x0010 ,L"Start" },
				{ 0x0020 ,L"Back" },
				{ 0x0040 ,L"LThumb" },
				{ 0x0080 ,L"RThumb" },
				{ 0x0100 ,L"LShoulder" },
				{ 0x0200 ,L"RShoulder" },
			};
			if (dictionary.find(code) != dictionary.end())
			{
				return Format(L"XInput_", userIndex, L"_", dictionary[code]);
			}

			return Format(L"XInput_", userIndex, L"_", code);
		}


		//キーボード入力からキー変更
		bool ChangeKeyForKeyboard(Key& key)
		{

			for (auto&& newKey : Input::GetActiveKeys())
			{
				if (newKey.clicked)
				{
					key = newKey;
					return true;
				}
			}
			return false;
		}
		//ゲームパッドからキー変更
		bool ChangeKeyForGamePad(Key& key)
		{
			for (auto&& gamepadInfo : Input::EnumerateGamepads())
			{
				const auto gamepad = Gamepad(gamepadInfo.index);
				if (gamepad.hasPOV)
				{
					using GetPov = decltype(&Gamepad::_get_povBackward);

					static Array<GetPov> buttons
					{
						&Gamepad::_get_povBackward,
						&Gamepad::_get_povForward,
						&Gamepad::_get_povLeft,
						&Gamepad::_get_povRight
					};
					for (const auto& button : buttons)
					{
						Key newKey = (gamepad.*button)();
						if (newKey.clicked)
						{
							key = newKey;
							return true;
						}
					}
				}
				for (unsigned int i = 0; i < gamepadInfo.num_buttons; ++i)
				{
					Key newKey = Gamepad(gamepadInfo.index).button(i);
					if (newKey.clicked)
					{
						key = newKey;
						return true;
					}
				}
			}
			return false;
		}
		//XInputからキー変更
		bool ChangeKeyForXInput(Key& key)
		{
			for (uint8 userIndex = 0; userIndex < XInput::MaxUser; ++userIndex)
			{
				if (!XInput(userIndex).isConnected())
					break;

				using GetButton = decltype(&XInput::_get_buttonA);

				static Array<GetButton> buttons
				{
				&XInput::_get_buttonA,
				&XInput::_get_buttonB,
				&XInput::_get_buttonX,
				&XInput::_get_buttonY,
				&XInput::_get_buttonUp,
				&XInput::_get_buttonDown,
				&XInput::_get_buttonLeft,
				&XInput::_get_buttonRight,
				&XInput::_get_buttonStart,
				&XInput::_get_buttonBack,
				&XInput::_get_buttonLThumb,
				&XInput::_get_buttonRThumb,
				&XInput::_get_buttonLB,
				&XInput::_get_buttonRB
				};
				for (auto&& button : buttons)
				{
					auto newKey = (XInput(userIndex).*button)();
					if (newKey.clicked)
					{
						key = newKey;
						return true;
					}
				}
			}
			return false;
		}

		//キー変更
		bool ChangeKey(Key& key)
		{
			bool onSet = false;
			onSet |= ChangeKeyForKeyboard(key);
			onSet |= ChangeKeyForGamePad(key);
			onSet |= ChangeKeyForXInput(key);

			return onSet;
		}
	}

	/***************************************
	KeyConfig
	***************************************/
	KeyConfig::KeyConfig() :
		m_onConfigStart([]() {return Input::KeyEnter.clicked; }),
		m_onDelete([]() {return Input::KeyDelete.clicked; }),
		m_isSetting(false)
	{}

	KeyConfig::State KeyConfig::update(Key& key, bool useStrict)
	{
		if (this->m_isSetting)
		{
			if (ChangeKey(key))
			{
				if (useStrict && InArray(key, m_strictKeys))
				{
					key = Key();
				}
				m_isSetting = false;
				return State::OnChange;
			}
			return State::IsSetting;
		}
		else
		{
			if (m_onConfigStart())
			{
				m_isSetting = true;
				return State::OnStart;
			}
			else if (m_onDelete())//デリートキーを押せば設定消去
			{
				key = s3d::Key();
				return State::OnDelete;
			}
		}
		return State::Normal;
	}

	bool KeyConfig::isSetting() const
	{
		return m_isSetting;
	}

	void KeyConfig::registerStrictKeys(const std::initializer_list<Key>& keys)
	{
		m_strictKeys.assign(keys.begin(), keys.end());
	}

	void KeyConfig::setOnChangeStart(std::function<bool()> onStart)
	{
		m_onConfigStart = onStart;
	}

	void KeyConfig::setOnDelete(std::function<bool()> onDelete)
	{
		m_onDelete = onDelete;
	}


	//キーの名前取得
	const String GetKeyName(const Key& key)
	{
		switch (key.device)
		{
		case KeyDeviceType::Keyboard:return GetKeyboardName(key.code);
		case KeyDeviceType::Gamepad:return GetGamepadName(key.userIndex, key.code);
		case KeyDeviceType::XInput:return GetXInputName(key.userIndex, key.code);

		default:
			break;
		}
		return L"----";
	}

}//namespace s3dkc