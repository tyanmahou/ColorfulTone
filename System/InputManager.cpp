#include<Siv3D/Key.hpp>
#include"InputManager.hpp"
#include"PlayKey.h"
class InputManager::Impl
{
private:
	bool m_isRedClicked = false;
	bool m_isBlueClicked = false;
	bool m_isYellowClicked = false;
public:
	void update()
	{
		m_isRedClicked = m_isBlueClicked = m_isYellowClicked = false;

		m_isRedClicked = PlayKey::Red().clicked;
		m_isBlueClicked = PlayKey::Blue().clicked;
		m_isYellowClicked = PlayKey::Yellow().clicked;
	}

	bool isRedClicked() const
	{
		return m_isRedClicked;
	}

	void popRed()
	{
		m_isRedClicked = false;
	}

	bool isBlueClicked() const
	{
		return m_isBlueClicked;
	}

	void popBlue()
	{
		m_isBlueClicked = false;
	}

	void popYellow()
	{
		m_isYellowClicked = false;
	}

	bool isYellowClicked() const
	{
		return m_isYellowClicked;
	}

	bool isAnyClicked() const
	{
		return m_isRedClicked || m_isBlueClicked || m_isYellowClicked;
	}
};
InputManager::InputManager() :
	m_pImpl(std::make_shared<Impl>())
{}

bool InputManager::IsRedClicked()
{
	return Instance()->m_pImpl->isRedClicked();
}

void InputManager::PopRed()
{
	Instance()->m_pImpl->popRed();
}

bool InputManager::IsBlueClicked()
{
	return Instance()->m_pImpl->isBlueClicked();
}

void InputManager::PopBlue()
{
	Instance()->m_pImpl->popBlue();
}

bool InputManager::IsYellowClicked()
{
	return Instance()->m_pImpl->isYellowClicked();
}

void InputManager::PopYellow()
{
	Instance()->m_pImpl->popYellow();
}

bool InputManager::IsAnyClicked()
{
	return Instance()->m_pImpl->isAnyClicked();
}

void InputManager::Update()
{
	Instance()->m_pImpl->update();
}
