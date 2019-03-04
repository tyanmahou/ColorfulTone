#pragma once
#include<memory>

class KeyConfig
{
public:
	class Impl;
private:

	std::shared_ptr<Impl> m_pImpl;
public:

	KeyConfig();

	template<class Type>
	void changeMode();

	bool update();

	void draw()const;

};


