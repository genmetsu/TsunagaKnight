/*!
@file SoundObject.h
@brief サウンド
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	サウンドオブジェクト
	//--------------------------------------------------------------------------------------
	class SoundObject :public ObjectInterface, public ShapeInterface {
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//リソースキー
		wstring m_ResKey;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	ResKey	リソースキー

		*/
		//--------------------------------------------------------------------------------------
		SoundObject(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SoundObject();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オーディオリソースを得る
		@return	オーディオリソースのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<AudioResource> GetAudioResource() const;
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	再生開始。
		@param[in]	LoopCount = 0	ループ回数（XAUDIO2_LOOP_INFINITEで無限ループ）
		@param[in]	Volume = 1.0f	ボリューム
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Start(size_t LoopCount = 0, float Volume = 1.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	再生開始。
		@param[in]	Buffer	XAUDIO2_BUFFERの参照
		@param[in]	Volume = 1.0f	ボリューム
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Start(const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	再生ストップ。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Stop();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理。空関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。空関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	};
}
//end basecross