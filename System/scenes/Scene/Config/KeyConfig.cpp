#include <scenes/Scene/Config/KeyConfig.hpp>

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
                &s3d::detail::XInput_impl::buttonMenu,
                &s3d::detail::XInput_impl::buttonBack,
                &s3d::detail::XInput_impl::buttonView,
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
        case InputDeviceType::Keyboard:
        case InputDeviceType::Gamepad:
        case InputDeviceType::XInput:
            return key.name();

        default:
            break;
        }
        return U"----";
    }

}//namespace ct