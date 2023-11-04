#include <scenes/Scene/KeyConfig/KeyConfigManager.hpp>
#include <scenes/Scene/KeyConfig/KeyConfig.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
    //-----------------------------------------------------------------------

    class PlayKeyConfig;
    class DetailKeyConfig;
    //-----------------------------------------------------------------------


    //-----------------------------------------------------------------------
    //内部実装
    //-----------------------------------------------------------------------

    class KeyConfigManager::Impl
    {
    private:
        KeyConfigManager* m_pKeyConfig;
    protected:
        KeyConfig m_config;

        int m_timer;
        Font m_font;

        template<class Type>
        void changeMode()
        {
            m_pKeyConfig->changeMode<Type>();
        }
    public:
        Impl(int fontSize = 20) :
            m_timer(0),
            m_font(fontSize)
        {
            m_config.setOnChangeStart([]() {return PlayKey::Start().down(); });

        }

        virtual bool update() = 0;

        virtual void draw()const = 0;

        void setKeyConfig(KeyConfigManager* pKeyConfig)
        {
            this->m_pKeyConfig = pKeyConfig;
        }
    };

    //-----------------------------------------------------------------------
    //実際のゲームプレイに使うキーのコンフィグ
    //-----------------------------------------------------------------------

    class PlayKeyConfig :public KeyConfigManager::Impl
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
            switch (select) {
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
            switch (select) {
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
            switch (select) {
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
            switch (select) {
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

        void selectChange()
        {
            if (PlayKey::Right().down()) {
                SoundManager::PlaySe(U"select");
                m_select = moveRight(m_select);
            } else if (PlayKey::Left().down()) {
                SoundManager::PlaySe(U"select");
                m_select = moveLeft(m_select);
            } else if (PlayKey::Up().down()) {
                SoundManager::PlaySe(U"select");
                m_select = moveUp(m_select);
            } else if (PlayKey::Down().down()) {
                SoundManager::PlaySe(U"select");
                m_select = moveDown(m_select);
            }
        }

        Input* selectKey()const
        {
            auto& c = Game::Config();
            switch (m_select) {
            case Select::Red1:return &c.m_red1;
            case Select::Red2:return &c.m_red2;
            case Select::Red3: return &c.m_red3;

            case Select::Blue1:return &c.m_blue1;
            case Select::Blue2:return &c.m_blue2;
            case Select::Blue3:return &c.m_blue3;

            case Select::Yellow1:return &c.m_yellow1;
            case Select::Yellow2:return &c.m_yellow2;
            case Select::Yellow3:return &c.m_yellow3;

            default:
                break;
            }
            return nullptr;
        }
        bool update()override
        {
            ++m_timer;

            auto* pSelectKey = this->selectKey();
            if (pSelectKey) {
                auto state = m_config.update(*pSelectKey);

                if (state == KeyConfig::State::OnStart) {
                    SoundManager::PlaySe(U"desisionSmall");
                    return true;
                }
                if (state == KeyConfig::State::OnChange || state == KeyConfig::State::OnDelete) {
                    PlayKey::Update();
                    SoundManager::PlaySe(U"desisionSmall");
                    return true;
                }
            }
            if (PlayKey::Start().down()) {
                SoundManager::PlaySe(U"desisionSmall");

                if (m_select == Select::Detail) {
                    this->changeMode<DetailKeyConfig>();
                }
                if (m_select == Select::Default) {
                    PlayKey::Reset();
                }
                if (m_select == Select::Escape) {
                    return false;
                }
            }
            if (PlayKey::SmallBack().down())
                return false;

            if (m_config.isSetting()) {
                return true;
            }

            this->selectChange();

            return true;
        }

        void draw()const
        {
            TextureAsset(U"keyconBG2").draw();

            const auto getColor = [=](Select s) {
                if (m_select == s) {
                    if (m_config.isSetting())
                        return Color(255, 0, 0, 128 * ((m_timer / 10) % 2));
                    else
                        return Palette::Red;
                }

                return Palette::White;
                };

            auto& c = Game::Config();
            {
                constexpr int y = 155;
                m_font(GetKeyName(c.m_red1)).drawAt(170, y, getColor(Select::Red1));
                m_font(GetKeyName(c.m_red2)).drawAt(170, y + 60, getColor(Select::Red2));
                m_font(GetKeyName(c.m_red3)).drawAt(170, y + 120, getColor(Select::Red3));
            }
            {
                constexpr int y = 180;
                m_font(GetKeyName(c.m_blue1)).drawAt(400, 600 - y, getColor(Select::Blue1));
                m_font(GetKeyName(c.m_blue2)).drawAt(400, 600 - y + 60, getColor(Select::Blue2));
                m_font(GetKeyName(c.m_blue3)).drawAt(400, 600 - y + 120, getColor(Select::Blue3));
            }
            {
                constexpr int y = 155;
                m_font(GetKeyName(c.m_yellow1)).drawAt(800 - 170, y, getColor(Select::Yellow1));
                m_font(GetKeyName(c.m_yellow2)).drawAt(800 - 170, y + 60, getColor(Select::Yellow2));
                m_font(GetKeyName(c.m_yellow3)).drawAt(800 - 170, y + 120, getColor(Select::Yellow3));
            }
            m_font(U"詳細設定").drawAt(130, 490, getColor(Select::Detail));
            m_font(U"初期設定").drawAt(130, 540, getColor(Select::Default));

            m_font(U"戻る").drawAt(800 - 130, 525, getColor(Select::Escape));



            if (PlayKey::Red().pressed())
                TextureAsset(U"center_redlight").drawAt(400, 300);

            if (PlayKey::Blue().pressed())
                TextureAsset(U"center_bluelight").drawAt(400, 300);

            if (PlayKey::Yellow().pressed())
                TextureAsset(U"center_yellowlight").drawAt(400, 300);


            TextureAsset(U"center_base").drawAt(400, 300);

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
    class DetailKeyConfig :public KeyConfigManager::Impl
    {
        enum Select
        {
            Up, Down, Left, Right,
            Start, SmallBack, BigBack,
            SIZE
        };
    private:

        Select m_select;
        void selectChange()
        {
            if (PlayKey::Down().down()) {
                SoundManager::PlaySe(U"select");

                if (m_select < Select::SIZE - 1) {

                    m_select = static_cast<Select>(m_select + 1);
                }
            }
            if (PlayKey::Up().down()) {
                SoundManager::PlaySe(U"select");

                if (m_select > 0) {
                    m_select = static_cast<Select>(m_select - 1);
                }
            }
        }
        Input* selectKey()
        {
            auto& c = Game::Config();
            switch (m_select) {
            case Select::Up:		return &c.m_up;
            case Select::Down:		return &c.m_down;
            case Select::Left:		return &c.m_left;
            case Select::Right:		return &c.m_right;
            case Select::Start:		return &c.m_start;
            case Select::SmallBack:	return &c.m_smallBack;
            case Select::BigBack:	return &c.m_bigBack;

            default:
                break;
            }
            return nullptr;
        }
        bool update()override
        {
            ++m_timer;

            auto* pSelectKey = this->selectKey();
            if (pSelectKey) {
                auto state = m_config.update(*pSelectKey, true);
                if (state == KeyConfig::State::OnStart) {
                    SoundManager::PlaySe(U"desisionSmall");
                    return true;
                }
                if (state == KeyConfig::State::OnChange || state == KeyConfig::State::OnDelete) {
                    PlayKey::Update();
                    SoundManager::PlaySe(U"desisionSmall");
                    return true;
                }
            }
            if (PlayKey::SmallBack().down()) {
                SoundManager::PlaySe(U"cancel");
                this->changeMode<PlayKeyConfig>();
            }

            if (m_config.isSetting()) {
                return true;
            }

            this->selectChange();

            return true;
        }
        void draw()const override
        {
            Rect(50, 100, 700, 450).draw(ColorF(0, 0.7));

            const auto getColor = [=](Select s) {
                if (m_select == s) {
                    if (m_config.isSetting())
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
                    { U"↑", U"Up", GetKeyName(c.m_up), getColor(Select::Up) },
                    { U"↓", U"Down", GetKeyName(c.m_down), getColor(Select::Down) },
                    { U"←", U"Left", GetKeyName(c.m_left), getColor(Select::Left) },
                    { U"→", U"Right", GetKeyName(c.m_right), getColor(Select::Right) },
                    { U"決定", U"Enter", GetKeyName(c.m_start), getColor(Select::Start) },
                    { U"戻る(小)", U"Backspace", GetKeyName(c.m_smallBack), getColor(Select::SmallBack) },
                    { U"戻る(大)", U"Escape", GetKeyName(c.m_bigBack), getColor(Select::BigBack) },

                };
                int i = 0;
                for (auto&& elm : list) {
                    m_font(std::get<0>(elm)).drawAt(100, 125 + i * 50);
                    m_font(std::get<1>(elm)).drawAt(200, 125 + i * 50);
                    m_font(std::get<2>(elm)).drawAt(300, 125 + i * 50, std::get<3>(elm));

                    ++i;
                }
            }
        }
    public:
        DetailKeyConfig() :
            KeyConfigManager::Impl(15),
            m_select(Select::Up)
        {
            m_config.registerStrictKeys({
            KeyUp,
            KeyDown,
            KeyLeft,
            KeyRight,
            KeyEnter,
            KeyBackspace,
            KeyEscape,
            KeyDelete
                });
        }
    };

    //-----------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------

    KeyConfigManager::KeyConfigManager()
    {
        this->changeMode<PlayKeyConfig>();
    }

    bool KeyConfigManager::update()
    {
        return m_pImpl->update();
    }

    void KeyConfigManager::draw() const
    {
        m_pImpl->draw();
    }

    template<class Type>
    void KeyConfigManager::changeMode()
    {
        m_pImpl = std::make_shared<Type>();
        m_pImpl->setKeyConfig(this);
    }

    template void KeyConfigManager::changeMode<PlayKeyConfig>();
    template void KeyConfigManager::changeMode<DetailKeyConfig>();

}
