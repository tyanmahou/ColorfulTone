#include <core/Input/HighPrecision/HighPrecisionPlayer.hpp>
#include <Siv3D.hpp>

namespace ct
{
    HighPrecisionPlayer::HighPrecisionPlayer()
    {
    }

    bool HighPrecisionPlayer::isClicked(GameInputKind kind) const
    {
        return !m_clickTimes[static_cast<size_t>(kind)].empty();
    }

    bool HighPrecisionPlayer::isPressed(GameInputKind kind) const
    {
        return GetPlayKey(kind).pressed();
    }

    void HighPrecisionPlayer::pop(GameInputKind kind)
    {
        if (this->isClicked(kind)) {
            m_clickTimes[static_cast<size_t>(kind)].pop();
        }
    }

    s3d::int64 HighPrecisionPlayer::clickedMillisecOffset(GameInputKind kind) const
    {
        if (this->isClicked(kind)) {
            return -static_cast<int64>(m_time - m_clickTimes[static_cast<size_t>(kind)].front());
        }
        return 0;
    }

    void HighPrecisionPlayer::forceInput(GameInputKind kind, [[maybe_unused]] bool isLong)
    {
        size_t index = static_cast<size_t>(kind);
        if (m_clickTimes[index].empty()) {
            m_clickTimes[index].push(m_time);
        } else if (m_clickTimes[index].back() < m_time) {
            m_clickTimes[index].push(m_time);
        }
    }

    void HighPrecisionPlayer::update()
    {
        // clear
        for (size_t index = 0; index < 3; ++index) {
            std::queue<uint64>().swap(m_clickTimes[index]);
        }
        m_time = Time::GetMillisec();

        bool isClicked[3];
        for (size_t index = 0; index < 3; ++index) {
            isClicked[index] = GetPlayKey(index).down();
        }
        for (const auto& event : Platform::Windows::Keyboard::GetEvents()) {
            if (event.down && m_eventIndex < event.eventIndex) {
                m_eventIndex = event.eventIndex;

                for (size_t index = 0; index < 3; ++index) {
                    if (isClicked[index]) {
                        for (auto& input : GetPlayKey(index).inputs()) {
                            if (input.deviceType() == InputDeviceType::Keyboard && input.code() == event.code) {
                                if (m_clickTimes[index].empty()) {
                                    m_clickTimes[index].push(event.timeMillisec);
                                } else if (m_clickTimes[index].back() < event.timeMillisec) {
                                    m_clickTimes[index].push(event.timeMillisec);
                                }
                            }
                        }
                    }
                }
            }
        }
        for (size_t index = 0; index < 3; ++index) {
            if (isClicked[index] && m_clickTimes[index].empty()){
                m_clickTimes[index].push(m_time);
            }
        }
    }
}
