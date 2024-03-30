#include <core/Input/InputManager.hpp>
#include <core/Input/PlayKey.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class InputManager::Impl
    {
    public:
        void update()
        {
            std::queue<uint64>().swap(m_red);
            std::queue<uint64>().swap(m_blue);
            std::queue<uint64>().swap(m_yellow);

            m_time = Time::GetMillisec();

            bool redDown = PlayKey::Red().down();
            bool blueDown = PlayKey::Blue().down();
            bool yellowDown = PlayKey::Yellow().down();

            for (const auto& event : Platform::Windows::Keyboard::GetEvents()) {
                if (event.down && m_eventIndex < event.eventIndex) {
                    m_eventIndex = event.eventIndex;

                    if (redDown) {
                        for (auto& input : PlayKey::Red().inputs()) {
                            if (input.deviceType() == InputDeviceType::Keyboard && input.code() == event.code) {
                                m_red.push(event.timeMillisec);
                            }
                        }
                    }
                    if (blueDown) {
                        for (auto& input : PlayKey::Blue().inputs()) {
                            if (input.deviceType() == InputDeviceType::Keyboard && input.code() == event.code) {
                                m_blue.push(event.timeMillisec);
                            }
                        }
                    }
                    if (yellowDown) {
                        for (auto& input : PlayKey::Yellow().inputs()) {
                            if (input.deviceType() == InputDeviceType::Keyboard && input.code() == event.code) {
                                m_yellow.push(event.timeMillisec);
                            }
                        }
                    }
                }
            }
            if (redDown && m_red.empty()) {
                m_red.push(m_time);
            }
            if (blueDown && m_blue.empty()) {
                m_blue.push(m_time);
            }
            if (yellowDown && m_yellow.empty()) {
                m_yellow.push(m_time);
            }
        }

        bool isRedClicked() const
        {
            return !m_red.empty();
        }

        void popRed()
        {
            m_red.pop();
        }
        int64 redClickedTimeOffset() const
        {
            if (this->isRedClicked()) {
                return -static_cast<int64>(m_time - m_red.front());
            }
            return 0;
        }
        bool isBlueClicked() const
        {
            return !m_blue.empty();
        }

        void popBlue()
        {
            m_blue.pop();
        }

        int64 blueClickedTimeOffset() const
        {
            if (this->isBlueClicked()) {
                return -static_cast<int64>(m_time - m_blue.front());
            }
            return 0;
        }

        bool isYellowClicked() const
        {
            return !m_yellow.empty();
        }

        void popYellow()
        {
            m_yellow.pop();
        }
        int64 yellowClickedTimeOffset() const
        {
            if (this->isYellowClicked()) {
                return -static_cast<int64>(m_time - m_yellow.front());
            }
            return 0;
        }
        bool isAnyClicked() const
        {
            return this->isRedClicked() || this->isBlueClicked() || this->isYellowClicked();
        }
    private:
        std::queue<uint64> m_red;
        std::queue<uint64> m_blue;
        std::queue<uint64> m_yellow;

        uint64 m_time = 0;
        uint64 m_eventIndex = 0;
    };
    InputManager::InputManager() :
        m_pImpl(std::make_unique<Impl>())
    {}

    bool InputManager::IsRedClicked()
    {
        return Instance()->m_pImpl->isRedClicked();
    }

    void InputManager::PopRed()
    {
        Instance()->m_pImpl->popRed();
    }

    s3d::int64 InputManager::RedClickedTimeOffset()
    {
        return Instance()->m_pImpl->redClickedTimeOffset();
    }

    bool InputManager::IsBlueClicked()
    {
        return Instance()->m_pImpl->isBlueClicked();
    }

    void InputManager::PopBlue()
    {
        Instance()->m_pImpl->popBlue();
    }

    s3d::int64 InputManager::BlueClickedTimeOffset()
    {
        return Instance()->m_pImpl->blueClickedTimeOffset();
    }

    bool InputManager::IsYellowClicked()
    {
        return Instance()->m_pImpl->isYellowClicked();
    }

    void InputManager::PopYellow()
    {
        Instance()->m_pImpl->popYellow();
    }

    s3d::int64 InputManager::YellowClickedTimeOffset()
    {
        return Instance()->m_pImpl->yellowClickedTimeOffset();
    }

    bool InputManager::IsAnyClicked()
    {
        return Instance()->m_pImpl->isAnyClicked();
    }

    void InputManager::Update()
    {
        Instance()->m_pImpl->update();
    }
}
