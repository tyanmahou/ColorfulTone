#pragma once
#include <Siv3d.hpp>

class NotesData;

enum class BGType
{
	Default=0,
	Blur,
	Black,
	White
};

class IPlayBG
{
public:
	IPlayBG() = default;
	virtual ~IPlayBG() = default;
	virtual void init(const NotesData& notes)= 0;
	virtual void apply(double count=0)const = 0;
};

class DefaultBG:public IPlayBG
{
public:
	DefaultBG() = default;
private:
	TextureRegion m_texture;

	void init(const NotesData& notes)override;
	void apply(double count)const override;
};

class BlurBG :public IPlayBG
{
public:
	BlurBG();

private:

	RenderTexture m_texture;

	void init(const NotesData& notes)override;
	void apply(double count)const override;
};
class SimpleBG :public IPlayBG
{
public:
	SimpleBG(const Color& color = Palette::White);
private:
	Color m_color;
	TextureRegion m_texture;

	void init(const NotesData& notes)override;
	void apply(double count)const override;
};

inline std::shared_ptr<IPlayBG> CreateBG(BGType type = BGType::Default)
{
	switch (type)
	{
	case BGType::Default:
		return std::make_shared<DefaultBG>();
	case BGType::Blur:
		return std::make_shared<BlurBG>();
	case BGType::Black:
		return std::make_shared<SimpleBG>(Palette::Black);
	case BGType::White:
		return std::make_shared<SimpleBG>();
	default:
		break;
	}

	return std::make_shared<DefaultBG>();
}