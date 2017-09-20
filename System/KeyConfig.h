#pragma once
#include<Siv3D.hpp>


class KeyConfig
{
public:
	class Impl;
private:

	std::shared_ptr<Impl> m_pImpl;
	int m_timer;				//タイマー
public:

	KeyConfig();

	template<class Type>
	void changeMode();

	bool update();

	void draw()const;

};


