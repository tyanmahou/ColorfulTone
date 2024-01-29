#include <core/Input/AutoPlayManager.hpp>
#include <utils/Addon/IntervalCounter.hpp>
#include <array>

namespace ct
{
    class AutoPlayManager::Impl
    {
    private:
        struct FrameCount
        {
            FrameCount() = default;
            FrameCount(s3d::int32 _c = 15) :
                count(_c)
            {}
            s3d::int32 count = 0;
        };

        std::array<FrameCount, 3> m_isPressed;
    public:
        bool m_autoPlay;
        Impl() :
            m_autoPlay(false),
            m_isPressed{ 0,0,0 }
        {}

        bool isRedPressed()const
        {
            return m_isPressed[0].count;
        }
        bool isBluePressed()const
        {
            return m_isPressed[1].count;
        }
        bool isYellowPressed()const
        {
            return m_isPressed[2].count;
        }
        void update()
        {
            if (IntervalCounter::IsUpdated()) {
                for (auto&& elm : m_isPressed) {
                    if (elm.count > 0) {
                        --elm.count;
                    }
                }
            }
        }
        void input(NoteType type)
        {
            const int fc = type >= 11 && type <= 17 ? 1 : 7;

            switch (type) {
            case 1:
            case 11:m_isPressed[0] = FrameCount(fc); break;
            case 2:
            case 12:m_isPressed[1] = FrameCount(fc); break;
            case 3:
            case 13:m_isPressed[2] = FrameCount(fc); break;
            case 4:
            case 14:
                m_isPressed[1] = FrameCount(fc);
                m_isPressed[2] = FrameCount(fc);
                break;
            case 5:
            case 15:
                m_isPressed[0] = FrameCount(fc);
                m_isPressed[2] = FrameCount(fc);
                break;
            case 6:
            case 16:
                m_isPressed[0] = FrameCount(fc);
                m_isPressed[1] = FrameCount(fc);
                break;
            case 7:
            case 17:
                m_isPressed[0] = FrameCount(fc);
                m_isPressed[1] = FrameCount(fc);
                m_isPressed[2] = FrameCount(fc);
            }
        }
    };

    AutoPlayManager::AutoPlayManager() :
        m_pImpl(std::make_unique<Impl>())
    {}

    void AutoPlayManager::ChangePlayMode()
    {
        bool& isAuto = Instance()->m_pImpl->m_autoPlay;
        isAuto = !isAuto;
    }

    void AutoPlayManager::SetAutoPlay(bool isAuto)
    {
        Instance()->m_pImpl->m_autoPlay = isAuto;
    }

    bool AutoPlayManager::IsAutoPlay()
    {
        return Instance()->m_pImpl->m_autoPlay;
    }

    bool AutoPlayManager::IsRedPressed()
    {
        return Instance()->m_pImpl->isRedPressed();
    }

    bool AutoPlayManager::IsBluePressed()
    {
        return Instance()->m_pImpl->isBluePressed();
    }
    bool AutoPlayManager::IsYellowPressed()
    {
        return Instance()->m_pImpl->isYellowPressed();
    }

    void AutoPlayManager::Update()
    {
        Instance()->m_pImpl->update();
    }

    void AutoPlayManager::Input(NoteType type)
    {
        Instance()->m_pImpl->input(type);
    }
}
