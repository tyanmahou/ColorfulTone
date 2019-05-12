#include "FileLoadView.hpp"
#include"BGLight.hpp"
#include"SharedDraw.hpp"
#include"FileLoad.h"

class FileLoadView::Impl
{
private:
	SharedDraw::DrawBGLight m_lights;
	SharedDraw::LoadingProgress m_loading;
public:
	Impl()
	{}
	void update()
	{
		m_lights.update();
		m_loading.update();
	}
	void draw()const
	{
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();
		m_loading.draw(FileLoad::GetProgress());
	}

	void onCompleted()
	{
		m_loading.onCompleted();
	}
	int32 getStopwatchMs() const
	{
		return m_loading.getStopwatchMs();
	}
};
FileLoadView::FileLoadView() :
	m_pImpl(std::make_shared<Impl>())
{}

FileLoadView::~FileLoadView()
{}

void FileLoadView::update()
{
	m_pImpl->update();
}
void FileLoadView::draw() const
{
	m_pImpl->draw();
}

void FileLoadView::onCompleted()
{
	m_pImpl->onCompleted();
}

int32 FileLoadView::getStopwatchMs() const
{
	return m_pImpl->getStopwatchMs();
}