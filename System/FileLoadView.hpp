#pragma once
#include<Siv3D.hpp>
// ロード画面ビュー
class FileLoadView
{
private:
	Stopwatch m_stopwatch;
	bool m_isCompleted = false;
	String m_message;
public:
	FileLoadView();
	~FileLoadView();
	void update();
	void draw()const;
	void onCompleted();
};

