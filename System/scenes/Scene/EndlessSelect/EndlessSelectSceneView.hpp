#pragma once
#include <memory>

namespace ct
{
	class EndlessSelectScene;

	class EndlessSelectSceneView
	{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		EndlessSelectSceneView(const EndlessSelectScene* const scene);
		~EndlessSelectSceneView() = default;

		void update() const;
		void onChangeAction()const;
		void draw()const;
	};
}
