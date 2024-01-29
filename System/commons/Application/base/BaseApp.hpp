#pragma once
#include <commons/Application/base/IApplication.hpp>
#include <commons/Application/WindowCtrl.hpp>

namespace ct
{
	class BaseApp : public IApplication
	{
	public:
		BaseApp(const s3d::String& appName, s3d::Size windowSize, bool useScalable = false);

		bool run();
	protected:
		WindowCtrl m_windowCtrl;
		bool m_showFps = false;
	};
}
