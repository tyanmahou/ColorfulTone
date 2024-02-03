#pragma once
#include <memory>

namespace ct
{
	class ResultScene;

	class ResultSceneView
	{
	public:
		ResultSceneView(const ResultScene* const scene);
		~ResultSceneView() = default;

		void init();
		void update() const;
		void draw() const;
	private:
		class Impl;
		std::shared_ptr<Impl> m_impl;
	};
}
