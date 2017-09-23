#pragma once
#include<Siv3D.hpp>
class SoundManager
{
public:
	class BGM :s3d::Uncopyable
	{

		float m_volume = 1.0;
		s3d::Array<String> m_soundKeys;

		BGM() = default;

		static BGM& I()
		{
			static BGM instance;
			return instance;
		}
	public:
		static bool Register(const s3d::AssetName& s)
		{
			auto& keys = I().m_soundKeys;
			if (s3d::AnyOf(keys, [&s](const String& str) {return str == s; }))
			{
				return false;
			}
			I().m_soundKeys.emplace_back(s);

			return true;
		}

		static void SetVolume(float volume)
		{
			I().m_volume = volume;
			for (auto&& key : I().m_soundKeys)
			{
				SoundAsset(key).setVolume(volume);
			}
		}
		static float GetVolume()
		{
			return I().m_volume;
		}
	};
	class SE :s3d::Uncopyable
	{
		float m_volume = 1.0;
		SE() = default;
		static SE& I()
		{
			static SE instance;
			return instance;
		}
	public:
		static void SetVolume(float volume)
		{
			I().m_volume = volume;
		}
		static float GetVolume()
		{
			return I().m_volume;
		}
		static void Play(const s3d::AssetName& name)
		{
			SoundAsset(name).playMulti(GetVolume());
		}
	};
};
