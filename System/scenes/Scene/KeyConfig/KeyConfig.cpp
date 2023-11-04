#include <scenes/Scene/KeyConfig/KeyConfig.hpp>

namespace ct
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
                { 0x00,U"----" },
                { 0x01,U"MouseL" },
                { 0x02,U"MouseR" },
                { 0x04,U"MouseM" },
                { 0x05,U"MouseX1" },
                { 0x06,U"MouseX2" },
                { 0x03,U"Cancel" },
                { 0x08,U"Backspace" },
                { 0x09,U"Tab" },
                { 0x0C,U"Clear" },
                { 0x0D,U"Enter" },
                { 0x10,U"Shift" },
                { 0x11,U"Control" },
                { 0x12,U"Alt" },
                { 0x13,U"Pause" },
                { 0x1B,U"Escape" },
                { 0x20,U"Space" },
                { 0x21,U"PageUp" },
                { 0x22,U"PageDown" },
                { 0x23,U"End" },
                { 0x24,U"Home" },
                { 0x25,U"Left" },
                { 0x26,U"Up" },
                { 0x27,U"Right" },
                { 0x28,U"Down" },
                { 0x2C,U"PrintScreen" },
                { 0x2D,U"Insert" },
                { 0x2E,U"Delete" },
                { 0xA0,U"LShift" },
                { 0xA1,U"RShift" },
                { 0xA2,U"LControl" },
                { 0xA3,U"RControl" },
                { 0xA4,U"LAlt" },
                { 0xA5,U"RAlt" },
                { 0x30,U"0" },
                { 0x31,U"1" },
                { 0x32,U"2" },
                { 0x33,U"3" },
                { 0x34,U"4" },
                { 0x35,U"5" },
                { 0x36,U"6" },
                { 0x37,U"7" },
                { 0x38,U"8" },
                { 0x39,U"9" },
                { 0x41,U"A" },
                { 0x42,U"B" },
                { 0x43,U"C" },
                { 0x44,U"D" },
                { 0x45,U"E" },
                { 0x46,U"F" },
                { 0x47,U"G" },
                { 0x48,U"H" },
                { 0x49,U"I" },
                { 0x4A,U"J" },
                { 0x4B,U"K" },
                { 0x4C,U"L" },
                { 0x4D,U"M" },
                { 0x4E,U"N" },
                { 0x4F,U"O" },
                { 0x50,U"P" },
                { 0x51,U"Q" },
                { 0x52,U"R" },
                { 0x53,U"S" },
                { 0x54,U"T" },
                { 0x55,U"U" },
                { 0x56,U"V" },
                { 0x57,U"W" },
                { 0x58,U"X" },
                { 0x59,U"Y" },
                { 0x5A,U"Z" },
                { 0x60,U"Num0" },
                { 0x61,U"Num1" },
                { 0x62,U"Num2" },
                { 0x63,U"Num3" },
                { 0x64,U"Num4" },
                { 0x65,U"Num5" },
                { 0x66,U"Num6" },
                { 0x67,U"Num7" },
                { 0x68,U"Num8" },
                { 0x69,U"Num9" },
                { 0x6A,U"NumMultiply" },
                { 0x6B,U"NumAdd" },
                { 0x6C,U"NumSeparator" },
                { 0x6D,U"NumSubtract" },
                { 0x6E,U"NumDecimal" },
                { 0x6F,U"NumDivide" },
                { 0x90,U"NumLock" },
                { 0x70,U"F1" },
                { 0x71,U"F2" },
                { 0x72,U"F3" },
                { 0x73,U"F4" },
                { 0x74,U"F5" },
                { 0x75,U"F6" },
                { 0x76,U"F7" },
                { 0x77,U"F8" },
                { 0x78,U"F9" },
                { 0x79,U"F10" },
                { 0x7A,U"F11" },
                { 0x7B,U"F12" },
                { 0xBA,U"Colon" },
                { 0xBB,U"Semicolon" },
                { 0xBC,U"Comma" },
                { 0xBD,U"Minus" },
                { 0xBE,U"Period" },
                { 0xBF,U"Slash" },
                { 0xC0,U"At" },
                { 0xDB,U"LBracket" },
                { 0xDC,U"Yen" },
                { 0xDD,U"RBracket" },
                { 0xDE,U"Caret" },
                { 0xE2,U"Backslash" },
                { 0xB0,U"NextTrack" },
                { 0xB1,U"PreviousTrack" },
                { 0xB2,U"StopMedia" },
                { 0xB3,U"PlayPauseMedia" }
            };
            if (dictionary.find(code) != dictionary.end()) {
                return dictionary.at(code);
            }

            return  Format(U"Keyboard_", code);
        }

        //ゲームパッドのキー名
        const String GetGamepadName(const uint8 userIndex, const uint8 code)
        {
            return Format(U"Gamepad_", userIndex, U"_", code);
        }

        //XInputのキー名
        const String GetXInputName(const uint8 userIndex, const uint8 code)
        {
            static std::unordered_map<uint8, String> dictionary
            {
                { 0x1000 ,U"A"},
                { 0x2000 ,U"B" },
                { 0x4000 ,U"X" },
                { 0x8000 ,U"Y" },
                { 0x0001 ,U"Up" },
                { 0x0002 ,U"Down" },
                { 0x0004 ,U"Left" },
                { 0x0008 ,U"Right" },
                { 0x0010 ,U"Start" },
                { 0x0020 ,U"Back" },
                { 0x0040 ,U"LThumb" },
                { 0x0080 ,U"RThumb" },
                { 0x0100 ,U"LShoulder" },
                { 0x0200 ,U"RShoulder" },
            };
            if (dictionary.find(code) != dictionary.end()) {
                return Format(U"XInput_", userIndex, U"_", dictionary[code]);
            }

            return Format(U"XInput_", userIndex, U"_", code);
        }


        //キーボード入力からキー変更
        bool ChangeKeyForKeyboard(Input& key)
        {

            for (auto&& newKey : Keyboard::GetAllInputs()) {
                if (newKey.down()) {
                    key = newKey;
                    return true;
                }
            }
            return false;
        }
        //ゲームパッドからキー変更
        bool ChangeKeyForGamePad(Input& key)
        {
            for (auto&& gamepadInfo : System::EnumerateGamepads()) {
                const s3d::detail::Gamepad_impl gamepad = Gamepad(gamepadInfo.playerIndex);
                if (!gamepad.isConnected()) {
                    continue;
                }
                using GetPov = decltype(&s3d::detail::Gamepad_impl::povDown);

                static Array<GetPov> buttons
                {
                    &s3d::detail::Gamepad_impl::povDown,
                    &s3d::detail::Gamepad_impl::povUp,
                    &s3d::detail::Gamepad_impl::povLeft,
                    &s3d::detail::Gamepad_impl::povRight
                };
                for (const auto& button : buttons) {
                    const Input& newKey = (gamepad.*button);
                    if (newKey.down()) {
                        key = newKey;
                        return true;
                    }
                }
                for (const auto& newKey : gamepad.buttons) {
                    if (newKey.down()) {
                        key = newKey;
                        return true;
                    }
                }
            }
            return false;
        }
        //XInputからキー変更
        bool ChangeKeyForXInput(Input& key)
        {
            for (uint8 userIndex = 0; userIndex < detail::XInput_helper::MaxPlayerCount; ++userIndex) {
                auto gamepad = XInput(userIndex);
                if (!gamepad.isConnected()) {
                    break;
                }

                using GetButton = decltype(&s3d::detail::XInput_impl::buttonA);

                static Array<GetButton> buttons
                {
                &s3d::detail::XInput_impl::buttonA,
                &s3d::detail::XInput_impl::buttonB,
                &s3d::detail::XInput_impl::buttonX,
                &s3d::detail::XInput_impl::buttonY,
                &s3d::detail::XInput_impl::buttonUp,
                &s3d::detail::XInput_impl::buttonDown,
                &s3d::detail::XInput_impl::buttonLeft,
                &s3d::detail::XInput_impl::buttonRight,
                &s3d::detail::XInput_impl::buttonStart,
                &s3d::detail::XInput_impl::buttonBack,
                &s3d::detail::XInput_impl::buttonLThumb,
                &s3d::detail::XInput_impl::buttonRThumb,
                &s3d::detail::XInput_impl::buttonLB,
                &s3d::detail::XInput_impl::buttonRB
                };
                for (auto&& button : buttons) {
                    const Input& newKey = (gamepad.*button);
                    if (newKey.down()) {
                        key = newKey;
                        return true;
                    }
                }
            }
            return false;
        }

        //キー変更
        bool ChangeKey(Input& key)
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
        m_onConfigStart([]() {return KeyEnter.down(); }),
        m_onDelete([]() {return KeyDelete.down(); }),
        m_isSetting(false)
    {}

    KeyConfig::State KeyConfig::update(Input& key, bool useStrict)
    {
        if (this->m_isSetting) {
            if (ChangeKey(key)) {
                if (useStrict && InArray(key, m_strictKeys)) {
                    key = Input();
                }
                m_isSetting = false;
                return State::OnChange;
            }
            return State::IsSetting;
        } else {
            if (m_onConfigStart()) {
                m_isSetting = true;
                return State::OnStart;
            } else if (m_onDelete())//デリートキーを押せば設定消去
            {
                key = s3d::Input();
                return State::OnDelete;
            }
        }
        return State::Normal;
    }

    bool KeyConfig::isSetting() const
    {
        return m_isSetting;
    }

    void KeyConfig::registerStrictKeys(const std::initializer_list<Input>& keys)
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
    const String GetKeyName(const Input& key)
    {
        switch (key.deviceType()) {
        case InputDeviceType::Keyboard:return GetKeyboardName(key.code());
        case InputDeviceType::Gamepad:return GetGamepadName(key.playerIndex(), key.code());
        case InputDeviceType::XInput:return GetXInputName(key.playerIndex(), key.code());

        default:
            break;
        }
        return U"----";
    }

}//namespace ct