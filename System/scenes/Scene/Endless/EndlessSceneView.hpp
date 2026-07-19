#pragma once
#include <memory>

namespace ct
{
	class EndlessScene;

	class EndlessSceneView
	{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		EndlessSceneView(const EndlessScene* const scene);

		~EndlessSceneView() = default;

		void update() const;
		void draw()const;
	};
}