#pragma once
#include<Siv3D/Fwd.hpp>
#include<memory>
// ロード画面ビュー


class FileLoadView
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	FileLoadView();
	~FileLoadView();
	void update();
	void draw()const;
	void onCompleted();

	s3d::int32 getStopwatchMs()const;
};

