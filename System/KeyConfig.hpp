#pragma once
#include<Siv3D.hpp>

namespace s3dkc
{
	///<summary>
	///キーコンフィグクラス
	///</summary>
	class KeyConfig
	{
	public:
		enum class State
		{
			Normal,		//通常時
			IsSetting,	//セッティング中
			OnChange,	//セット完了
			OnDelete,	//設定の消去時
		};
	private:
		Key m_configStartKey, m_deleteKey;
		Array<Key> m_strictKeys;
		bool m_isSetting;

	public:

		///<summary>
		///コンストラクタ
		///</summary>
		/// <param name="configStartKey">
		/// 変更開始時に入力するキー
		/// </param>
		/// <param name="deleteKey">
		/// 設定されたキーの消去に入力するキー
		/// </param>
		KeyConfig(const Key& configStartKey = Input::KeyEnter, const Key& deleteKey = Input::KeyDelete);

		/// <summary>
		/// キーコンフィグの更新
		/// </summary>
		/// <param name="key">
		/// 変更するキー
		/// </param>
		/// <param name="useStrict">
		/// 使用できないキーをチェックするか
		/// </param>
		State update(Key& key, bool useStrict = false);

		/// <summary>
		/// 変更中かどうか
		/// </summary>
		bool isSetting()const;

		/// <summary>
		/// 使用できないキーの登録
		/// </summary>
		/// <param name="key">
		/// 使用できないキー
		/// </param>
		void registerStrictKeys(const std::initializer_list<Key>& keys);
	};


	/// <summary>
	/// キーの名前取得
	/// </summary>
	const String GetKeyName(const Key& key);
}