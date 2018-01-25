/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {



	//--------------------------------------------------------------------------------------
	///	ゲームステージ
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//描画オブジェクトの追加
		void CreateDrawObjects();
		//以下、別に保存しておく
		//文字列描画オブジェクト
		shared_ptr<StringDrawObject> m_StringDrawObject;
		//複数使用する球体の登録（リソース登録する）
		void RegisterDefaultSphere();
		weak_ptr<Player> m_PtrPlayer;
		shared_ptr<MultiAudioObject> m_AudioObjectPtr;

		Vec3 m_CameraPos;

		float m_FrameCount;
		float m_StartCameraY;
		bool m_FogActive;
		bool m_isClear;
		bool m_isFail;
		bool m_isGameStart;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		GameStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameStage() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ更新（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ描画（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

		void CannonStateStartBehaviour();
		void CannonStateEndBehaviour();

		void GameClearBehaviour();
		void GameOverBehaviour();

		bool GetIsStart() {
			return m_isGameStart;
		}

		void SetIsClear(bool value) {
			m_isClear = value;
			GameClearBehaviour();
		}
		bool GetIsClear() {
			return m_isClear;
		}

		bool GetFogActive() {
			return m_FogActive;
		}

		void SetIsFail(bool value) {
			m_isFail = value;
			GameOverBehaviour();
		}
		bool GetIsFail() {
			return m_isFail;
		}

		void SetActiveObjects(bool value);


		shared_ptr<Player> GetPlayerPtr()const {
			auto shptr = m_PtrPlayer.lock();
			if (shptr) {
				return shptr;
			}
			return nullptr;
		}
		void SetPlayerPtr(const shared_ptr<Player> ptr) {
			m_PtrPlayer = ptr;
		}

	};


	//--------------------------------------------------------------------------------------
	///	空のステージ（メッセージのみある）
	//--------------------------------------------------------------------------------------
	class EmptyStage : public Stage {
		shared_ptr<MessageSprite> m_MessageSprite;		///<メッセージを表示するスプライト
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		EmptyStage() {}
		virtual ~EmptyStage() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ更新（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ描画（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};





}
//end basecross
